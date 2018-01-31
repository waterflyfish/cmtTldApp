/*
 * VideoAppConfiguration.cpp
 *
 *  Created on: Jan 9, 2017
 *      Author: patrick
 */

#include <algorithm>
#include <list>

#include "libxml++/libxml++.h"
#include "Configuration.h"
#include "tools.h"
#include "../../config/videoapp_config.h"

using namespace std;
using namespace xmlpp;

namespace va {

/******************************************************************************
 * Implementtion of Configuration
 ******************************************************************************/
Configuration::~Configuration() {
}

Configuration::Configuration() :
		pipeConf(CONF_PIPE, LOG_CONF), comConf(CONF_COM, LOG_CONF)
#ifdef DEBUG_BUILD
				, dbgConf(CONF_DBG, LOG_CONF)
#endif
{
	logger = (log4cxx::Logger::getLogger(LOG_CONF));
	mavlinkSystemId = -1;
	mavlinkComponentId = -1;

}

bool Configuration::init(string configFilePath) {

	this->configFilePath = configFilePath;
	LOG4CXX_INFO(logger, "Using config file at: " << configFilePath);
	bool reVal = init();

	return reVal;
}

bool Configuration::init() {
	DomParser* parser = new DomParser();
	parser->parse_file(configFilePath);
	if (!parser) {
		LOG4CXX_ERROR(logger, "Cannot access config parser");
		delete parser;
		return false;
	} else {
		Document* conf = parser->get_document();
		const auto rootNode = conf->get_root_node();
		systemControllerId = dynamic_cast<Element*>(rootNode)->get_attribute_value("id");
		Node::NodeList mavNodes = rootNode->get_children("mav");
		for (auto mavNode : mavNodes) {
			Element* mavElem = dynamic_cast<Element*>(mavNode);
			string mavSysIdStr = mavElem->get_attribute_value("sysId");
			string mavCompIdStr = mavElem->get_attribute_value("compId");
			try {
				mavlinkSystemId = stoi(mavSysIdStr.c_str());
				mavlinkComponentId = stoi(mavCompIdStr.c_str());
			} catch (...) {
				LOG4CXX_ERROR(logger, "Error reading mavlink params");
				delete parser;
				return false;
			}
			Node::NodeList statusNodes = mavNode->get_children("status");
			for (auto statusNode : statusNodes) {
				Element* statusElem = dynamic_cast<Element*>(statusNode);
				string msgIdStr = statusElem->get_attribute_value("msgId");
				string intervalStr = statusElem->get_attribute_value("int");
				try {
					int msgId = stoi(msgIdStr.c_str());
					int interval = stoi(intervalStr.c_str());
					statusMessagesGenerationConfig.insert(pair<int, int>(msgId, interval));
				} catch (...) {
					LOG4CXX_ERROR(logger, "Error reading mavlink params");
					delete parser;
					return false;
				}
			}
		}
		LOG4CXX_DEBUG(logger, "VideoApp [mavlinkSystemId/mavlinkComponentId] ---> [" << mavlinkSystemId << "/" << mavlinkComponentId << "]");
		Node::NodeList subSysNodes = rootNode->get_children("subsystem");
		for (auto subSysNode : subSysNodes) {
			Element* subSysElem = dynamic_cast<Element*>(subSysNode);
			string subSysType = subSysElem->get_attribute_value("type");
			switch (str2int(subSysType.c_str())) {
			case str2int("com"):
				if (!comConf.init(subSysElem->get_attribute_value("conf"))) {
					LOG4CXX_ERROR(logger, "Error initializing CommunicationsController configuration");
					delete parser;
					return false;
				}
				break;
			case str2int("pipe"):
				if (!pipeConf.init(subSysElem->get_attribute_value("conf"))) {
					LOG4CXX_ERROR(logger, "Error initializing PipelineController configuration");
					delete parser;
					return false;
				}
				break;
			default:
				break;
			}
		}
#ifdef DEBUG_BUILD
		NodeSet dbgNodes = rootNode->find(string("//*[name()='").append("debug").append("']"));
		for (auto dbgSysNode : dbgNodes) {
			Element* subSysElem = dynamic_cast<Element*>(dbgSysNode);
			if (!dbgConf.init(subSysElem->get_attribute_value("conf"))) {
				LOG4CXX_ERROR(logger, "Error initializing debug configuration");
				return false;
			}
		}
#endif
		LOG4CXX_INFO(logger, "Reading configuration from [" << configFilePath << "] - " << SUCCESS);
	}
	delete parser;
	this->setObjectName(CONF_SYS);
	return true;
}

/******************************************************************************
 * Implementtion of Configuration::PipelineConfiguration
 ******************************************************************************/
Configuration::PipelineConfiguration::PipelineConfiguration(string objName, string loggerName) :
		Logable(objName, loggerName), vxOt_bbDecreasingratio(VXOT_BB_DECREASING_RATIO_DEF), vxOt_pyrLevels(VXOT_PYR_LEVELS_DEF), vxOt_lkNumIters(
		VXOT_LK_NUM_ITERS_DEF), vxOt_lkWinSize(VXOT_LK_WIN_SIZE_DEF), vxOt_detectorCellSize(VXOT_DETECTOR_CELL_SIZE_DEF), vxOt_maxCorners(
		VXOT_MAX_CORNERS_DEF), vxOt_fastType(VXOT_FAST_TYPE_DEF), vxOt_fastThreshold(VXOT_FAST_THRESHOLD_DEF), vxOt_harrisK(
		VXOT_HARRIS_K_DEF), vxOt_harrisThreshold(VXOT_HARRIS_THRESHOLD_DEF), vxOt_maxCornersInCell(VXOT_MAX_CORNERS_IN_CELL_DEF), vxOt_xNumOfCells(
		VXOT_X_NUM_OF_CELLS_DEF), vxOt_yNumOfCells(VXOT_Y_NUM_OF_CELLS_DEF), vxOt_useFastDetector(VXOT_USE_FAST_DETECTOR_DEF) {

}

Configuration::PipelineConfiguration::~PipelineConfiguration() {

}

bool Configuration::PipelineConfiguration::init(string confFilePath) {
	this->configFilePath = confFilePath;
	LOG4CXX_INFO(logger, "Using config file at: " << configFilePath);
	DomParser* parser = new DomParser();
	parser->parse_file(configFilePath);
	if (!parser) {
		LOG4CXX_ERROR(logger, "Cannot access config parser");
		delete parser;
		return false;
	} else {
		Document* conf = parser->get_document();
		const auto rootNode = conf->get_root_node();
		pipelineControllerId = dynamic_cast<Element*>(rootNode)->get_attribute_value("id");
		Node::NodeList targetNodes = rootNode->get_children("target");
		for (auto targetNode : targetNodes) {
			string ip = dynamic_cast<Element*>(targetNode)->get_attribute_value("ip");
			string portStr = dynamic_cast<Element*>(targetNode)->get_attribute_value("port");
			LOG4CXX_DEBUG(logger, "pipeline target is [ " << ip << "/" <<portStr);
			try {
				int port = stoi(portStr.c_str());
				streamTarget = pair<string, int>(ip, port);
				LOG4CXX_DEBUG(logger, "targetIp/targetPort: " << ip << "/" << port);
			} catch (...) {
				LOG4CXX_ERROR(logger, "Error reading target params");
				delete parser;
				return false;
			}
		}
		Node::NodeList vxOtNodes = rootNode->get_children("vx_ot");
		for (auto vxOtNode : vxOtNodes) {
			Element* vxOtElem = dynamic_cast<Element*>(vxOtNode);
			string vxOt_bb_decreasing_ratio = vxOtElem->get_attribute_value("bb_decreasing_ratio");
			string vxOt_pyr_levels = vxOtElem->get_attribute_value("pyr_levels");
			string vxOt_lk_num_iters = vxOtElem->get_attribute_value("lk_num_iters");
			string vxOt_lk_win_size = vxOtElem->get_attribute_value("lk_win_size");
			string vxOt_detector_cell_size = vxOtElem->get_attribute_value("detector_cell_size");
			string vxOt_max_corners = vxOtElem->get_attribute_value("max_corners");
			string vxOt_fast_type = vxOtElem->get_attribute_value("fast_type");
			string vxOt_fast_threshold = vxOtElem->get_attribute_value("fast_threshold");
			string vxOt_harris_k = vxOtElem->get_attribute_value("harris_k");
			string vxOt_harris_threshold = vxOtElem->get_attribute_value("harris_threshold");
			string vxOt_max_corners_in_cell = vxOtElem->get_attribute_value("max_corners_in_cell");
			string vxOt_x_num_of_cells = vxOtElem->get_attribute_value("x_num_of_cells");
			string vxOt_y_num_of_cells = vxOtElem->get_attribute_value("y_num_of_cells");
			string vxOt_use_fast_detector = vxOtElem->get_attribute_value("use_fast_detector");

			try {
				vxOt_bbDecreasingratio = stof(vxOt_bb_decreasing_ratio.c_str());
				vxOt_pyrLevels = stoi(vxOt_pyr_levels.c_str());
				vxOt_lkNumIters = stof(vxOt_lk_num_iters.c_str());
				vxOt_lkWinSize = stof(vxOt_lk_win_size.c_str());
				vxOt_detectorCellSize = stof(vxOt_detector_cell_size.c_str());
				vxOt_maxCorners = stof(vxOt_max_corners.c_str());
				vxOt_fastType = stof(vxOt_fast_type.c_str());
				vxOt_fastThreshold = stof(vxOt_fast_threshold.c_str());
				vxOt_harrisK = stof(vxOt_harris_k.c_str());
				vxOt_harrisThreshold = stof(vxOt_harris_threshold.c_str());
				vxOt_maxCornersInCell = stof(vxOt_max_corners_in_cell.c_str());
				vxOt_xNumOfCells = stof(vxOt_x_num_of_cells.c_str());
				vxOt_yNumOfCells = stof(vxOt_y_num_of_cells.c_str());
				int vxOt_useFastDetectorInt = stoi(vxOt_use_fast_detector.c_str());
				if (vxOt_useFastDetectorInt == 1) {
					vxOt_useFastDetector = true;
				} else {
					vxOt_useFastDetector = false;
				}
			} catch (...) {
				LOG4CXX_ERROR(logger, "Error reading vx_ot params");
				delete parser;
				return false;
			}
		}
		LOG4CXX_INFO(logger, "Reading configuration from [" << configFilePath << "] - " << SUCCESS);
		delete parser;
		bool reVal = true;
		return reVal;
	}
}

/******************************************************************************
 * Implementtion of Configuration::PipelineConfiguration
 ******************************************************************************/
Configuration::CommunicationsConfiguration::CommunicationsConfiguration(string objName, string loggerName) :
		Logable(objName, loggerName) {

}

Configuration::CommunicationsConfiguration::~CommunicationsConfiguration() {

}

bool Configuration::CommunicationsConfiguration::init(string confFilePath) {
	this->configFilePath = confFilePath;
	LOG4CXX_INFO(logger, "Using config file at: " << configFilePath);
	DomParser* parser = new DomParser();
	parser->parse_file(configFilePath);
	if (!parser) {
		LOG4CXX_ERROR(logger, "Cannot access config parser");
		delete parser;
		return false;
	} else {
		Document* conf = parser->get_document();
		const auto rootNode = conf->get_root_node();
		communicationsControllerId = dynamic_cast<Element*>(rootNode)->get_attribute_value("id");
		Node::NodeList chanNodes = rootNode->get_children("channel");
		for (auto chanNode : chanNodes) {
			Element* chanElem = dynamic_cast<Element*>(chanNode);
			string chanId = chanElem->get_attribute_value("id");
			string chanType = chanElem->get_attribute_value("type");
			switch (str2int(chanType.c_str())) {
			case str2int("udp"): {
				LOG4CXX_DEBUG(logger, "setting up udp: " << chanId);
				pair<string, int> targetCfg;
				Node::NodeList targetNodes = chanNode->get_children("target");
				for (auto targetNode : targetNodes) {
					string ip = dynamic_cast<Element*>(targetNode)->get_attribute_value("ip");
					string portStr = dynamic_cast<Element*>(targetNode)->get_attribute_value("port");
					try {
						int port = stoi(portStr.c_str());
						targetCfg = pair<string, int>(ip, port);
						LOG4CXX_DEBUG(logger, "targetIp/targetPort: " << ip << "/" << port);
					} catch (...) {
						LOG4CXX_ERROR(logger, "Error reading target params");
						delete parser;
						return false;
					}
				}
				pair<string, int> listenCfg;
				Node::NodeList listenNodes = chanNode->get_children("listen");
				for (auto listenNode : listenNodes) {
					string ip = dynamic_cast<Element*>(listenNode)->get_attribute_value("ip");
					string portStr = dynamic_cast<Element*>(listenNode)->get_attribute_value("port");
					try {
						int port = stoi(portStr.c_str());
						listenCfg = pair<string, int>(ip, port);
						LOG4CXX_DEBUG(logger, "listenip/listenport " << ip << "/" << port);
					} catch (...) {
						LOG4CXX_ERROR(logger, "Error reading target params");
						delete parser;
						return false;
					}
				}
				multimap<int, pair<int, int>> msgCfgs;
				Node::NodeList statusNodes = chanNode->get_children("status");
				for (auto statusNode : statusNodes) {
					Node::NodeList msgNodes = statusNode->get_children("msg");
					for (auto msgNode : msgNodes) {
						Element* msgElem = dynamic_cast<Element*>(msgNode);
						string msgIdStr = msgElem->get_attribute_value("id");
						string mavSysIdStr = msgElem->get_attribute_value("sysId");
						string mavCompIdStr = msgElem->get_attribute_value("compId");
						LOG4CXX_DEBUG(logger, "msg set: [" << msgIdStr << "/" << mavSysIdStr << "/" << mavCompIdStr << "]");
						try {
							int msgId = stoi(msgIdStr.c_str());
							int mavSysId = stoi(mavSysIdStr.c_str());
							int mavCompId = stoi(mavCompIdStr.c_str());
							msgCfgs.insert(pair<int, pair<int, int>>(msgId, pair<int, int>(mavSysId, mavCompId)));
						} catch (...) {
							LOG4CXX_ERROR(logger, "Error reading msg-mavlink params");
							delete parser;
							return false;
						}
					}
				}
				udpChannelConfigurations.insert(
						pair<string, pair<pair<pair<string, int>, pair<string, int>>, multimap<int, pair<int, int>>> >(chanId, pair<pair<pair<string, int>, pair<string, int>>, multimap<int, pair<int, int>>>(pair<pair<string, int>, pair<string, int>>(listenCfg, targetCfg), msgCfgs)));
			}
			break;

			case str2int("fifo"): {
				LOG4CXX_DEBUG(logger, "setting up fifo: " << chanId);
				//TODO /read/add the fifo config here
			}
			break;
			//will this ever be neccessary?
//			case str2int("serial"): {
//				LOG4CXX_DEBUG(logger, "setting up serial: " << chanId);
//			}
			break;
			default:
			LOG4CXX_WARN(logger, "ChanType [" << chanType << "] for channel [" << chanId << "] unknown! Config not read!");
			break;
		}
	}

		LOG4CXX_INFO(logger, "Reading configuration from [" << configFilePath << "] - " << SUCCESS);

	}
	delete parser;
	return true;
}

#ifdef DEBUG_BUILD
/******************************************************************************
 * Implementtion of Configuration::PipelineConfiguration
 ******************************************************************************/
Configuration::DebugConfiguration::DebugConfiguration(string objName, string loggerName) :
		Logable(objName, loggerName), enableTracking(true) {


	configFilePath = "";
	enableTracking = 1;
	filePipeFilePath = "";
	pair<pair<int, int>, pair<int,int>> vxOt_preDefinedRectangle = pair<pair<int, int>, pair<int,int>>(pair<int,int>(0,0), pair<int,int>(0,0));
}

Configuration::DebugConfiguration::~DebugConfiguration() {

}

bool Configuration::DebugConfiguration::init(string confFilePath) {
	this->configFilePath = confFilePath;
	LOG4CXX_INFO(logger, "Using config file at: " << configFilePath);
	DomParser* parser = new DomParser();
	parser->parse_file(configFilePath);
	if (!parser) {
		LOG4CXX_ERROR(logger, "Cannot access config parser");
		delete parser;
		return false;
	} else {
		Document* conf = parser->get_document();
		const auto rootNode = conf->get_root_node();
		Node::NodeList fileNodes = rootNode->get_children("file");
		for (auto fileNode : fileNodes) {

			filePipeFilePath = dynamic_cast<Element*>(fileNode)->get_attribute_value("path");
		}
		Node::NodeList vw_otNodes = rootNode->get_children("vw_ot");
		for (auto vw_otNode : vw_otNodes) {
			//LOG4CXX_ERROR(logger, "OOOOOOOOOOOO");
			Node::NodeList trackingNodes = vw_otNode->get_children("tracking");
			for (auto trackingNode : trackingNodes) {
				//LOG4CXX_ERROR(logger, "XXXXX");
				string enableTrackingStr = dynamic_cast<Element*>(trackingNode)->get_attribute_value("enable");
				try {
					int enableTrackingInt = stoi(enableTrackingStr.c_str());
					if (enableTrackingInt == 1) {
						enableTracking = true;
					} else {
						enableTracking = false;
					}
				} catch (...) {
					LOG4CXX_ERROR(logger, "Error reading debug params");
					delete parser;
					return false;
				}
			}
			Node::NodeList preObjNodes = vw_otNode->get_children("pre_obj");
			for (auto preObjNode : preObjNodes) {
				//LOG4CXX_ERROR(logger, "ZZZZZZ");
				Node::NodeList tlNodes = preObjNode->get_children("tl");
				pair<int, int> tl;
				pair<int, int> br;
				for (auto tlNode : tlNodes) {
					//LOG4CXX_ERROR(logger, "TTTTTTTTTT");
					string xStr = dynamic_cast<Element*>(tlNode)->get_attribute_value("x");
					string yStr = dynamic_cast<Element*>(tlNode)->get_attribute_value("y");
					try {
						int x = stoi(xStr.c_str());
						int y = stoi(yStr.c_str());
						tl = pair<int, int>(x, y);
					} catch (...) {
						LOG4CXX_ERROR(logger, "Error reading debug params");
						delete parser;
						return false;
					}
				}
				Node::NodeList brNodes = preObjNode->get_children("br");
				for (auto brNode : brNodes) {
					//LOG4CXX_ERROR(logger, "BBBBBBBBB");
					string xStr = dynamic_cast<Element*>(brNode)->get_attribute_value("x");
					string yStr = dynamic_cast<Element*>(brNode)->get_attribute_value("y");
					try {
						int x = stoi(xStr.c_str());
						int y = stoi(yStr.c_str());
						br = pair<int, int>(x, y);
					} catch (...) {
						LOG4CXX_ERROR(logger, "Error reading debug params");
						delete parser;
						return false;
					}
				}
				vxOt_preDefinedRectangle = pair<pair<int, int>, pair<int, int>>(tl, br);
			}
		}
	}
	LOG4CXX_INFO(logger, "Reading configuration from [" << configFilePath << "] - " << SUCCESS);
	delete parser;
	return true;
}
#endif
}
/* namespace va */

