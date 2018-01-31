/*
 * Configuration.h
 *
 *  Created on: Jan 9, 2017
 *      Author: patrick
 */

#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include <map>

#include "../tools/Logable.h"
#define DEBUG_BUILD 1
#define DEBUG_COUNTER 1
#define ENABLE_MAV_DEBUG_MSG 1
#define MAV_DEBUG_MSG_IN 5
namespace va {

class Configuration: public Logable {
public:
	virtual ~Configuration();

	static Configuration& getInstance() {
		static Configuration instance;
		return instance;
	}
	;
	Configuration(Configuration const&) = delete;
	void operator=(Configuration const&) = delete;

	bool init();
	bool init(string configFilePath);

	const string& getConfigFilePath() const {
		return configFilePath;
	}
	const string& getSystemControllerId() const {
		return systemControllerId;
	}
	int getMavlinkComponentId() const {
		return mavlinkComponentId;
	}
	int getMavlinkSystemId() const {
		return mavlinkSystemId;
	}
private:
	Configuration();
	string configFilePath;
	string systemControllerId;
	int mavlinkSystemId;
	int mavlinkComponentId;
	// msgid interval
	map<int, int> statusMessagesGenerationConfig;
	class PipelineConfiguration: public Logable {
	public:
		PipelineConfiguration(string objName, string loggerName);
		virtual ~PipelineConfiguration();
		bool init(string configFilePath);
		const string& getConfigFilePath() const {
			return configFilePath;
		}
		const string& getPipelineControllerId() const {
			return pipelineControllerId;
		}
		const pair<string, int>& getStreamTarget() const {
			return streamTarget;
		}
		float getVxOt_bbDecreasingratio() const {
			return vxOt_bbDecreasingratio;
		}
		int getVxOt_pyrLevels() const {
			return vxOt_pyrLevels;
		}
		int getVxOt_lkNumIters() const {
			return vxOt_lkNumIters;
		}
		int getVxOt_lkWinSize() const {
			return vxOt_lkWinSize;
		}
		int getVxOt_maxCorners() const {
			return vxOt_maxCorners;
		}
		int getVxOt_detectorCellSize() const {
			return vxOt_detectorCellSize;
		}
		int getVxOt_fastType() const {
			return vxOt_fastType;
		}
		int getVxOt_fastThreshold() const {
			return vxOt_fastThreshold;
		}
		float getVxOt_harrisK() const {
			return vxOt_harrisK;
		}
		float getVxOt_harrisThreshold() const {
			return vxOt_harrisThreshold;
		}
		int getVxOt_maxCornersInCell() const {
			return vxOt_maxCornersInCell;
		}
		int getVxOt_xNumOfCells() const {
			return vxOt_xNumOfCells;
		}
		int getVxOt_yNumOfCells() const {
			return vxOt_yNumOfCells;
		}
		bool getVxOt_useFastDetector() const {
			return vxOt_useFastDetector;
		}
	private:
		string configFilePath;
		string pipelineControllerId;
		pair<string, int> streamTarget;
		float vxOt_bbDecreasingratio;
		int vxOt_pyrLevels;
		int vxOt_lkNumIters;
		int vxOt_lkWinSize;
		int vxOt_detectorCellSize;
		int vxOt_maxCorners;
		int vxOt_fastType;
		int vxOt_fastThreshold;
		float vxOt_harrisK;
		float vxOt_harrisThreshold;
		int vxOt_maxCornersInCell;
		int vxOt_xNumOfCells;
		int vxOt_yNumOfCells;
		bool vxOt_useFastDetector;
	};
	class CommunicationsConfiguration: public Logable {
	public:
		CommunicationsConfiguration(string objName, string loggerName);
		virtual ~CommunicationsConfiguration();
		bool init(string configFilePath);
		const string& getConfigFilePath() const {
			return configFilePath;
		}
		const map<string, pair<pair<pair<string, int>, pair<string, int>>, multimap<int, pair<int, int>>> >&getUdpChannelConfigurations() const {
			return udpChannelConfigurations;
		}
		const string& getCommunicationsControllerId() const
		{
			return communicationsControllerId;
		}
	private:
		string configFilePath;
		string communicationsControllerId;
		// 	chanID                 ip    port          ip     port          msgId   sysid compid
		map<string, pair<pair<pair<string, int>, pair<string, int>>, multimap<int, pair<int, int>>>> udpChannelConfigurations;
	};
#ifdef DEBUG_BUILD
	class DebugConfiguration: public Logable {
	public:
		DebugConfiguration(string objName, string loggerName);
		virtual ~DebugConfiguration();
		bool init(string configFilePath);
		const string& getConfigFilePath() const {
			return configFilePath;
		}
		const string& getFilePipeFilePath() const
		{
			return filePipeFilePath;
		}
		bool getEnableTracking() const {
			return enableTracking;
		}
		const pair<pair<int, int>, pair<int,int>>& getVvxOt_preDefinedRectangle() const {
			return vxOt_preDefinedRectangle;
		}
	private:
		string configFilePath;
		bool enableTracking;
		string filePipeFilePath;
		pair<pair<int, int>, pair<int,int>> vxOt_preDefinedRectangle;
	};
#endif
	PipelineConfiguration pipeConf;
	CommunicationsConfiguration comConf;
#ifdef DEBUG_BUILD
	DebugConfiguration dbgConf;
#endif
public:
	const map<string, pair<pair<pair<string, int>, pair<string, int>>, multimap<int, pair<int, int>>>>& com_getUdpChannelConfigurations() const {
		return comConf.getUdpChannelConfigurations();
	}
	const string& getPipelineControllerId() const {
		return pipeConf.getPipelineControllerId();
	}

	const string& getCommunicationsControllerId() const
	{
		return comConf.getCommunicationsControllerId();
	}

	const map<int,int>& getStatusMessagesGenerationConfig() const
	{
		return statusMessagesGenerationConfig;
	}
	const pair<string, int>& pipe_getStreamTarget() const {
		return pipeConf.getStreamTarget();
	}
	float pipe_getVxOt_bbDecreasingratio() const {
		return pipeConf.getVxOt_bbDecreasingratio();
	}
	int pipe_getVxOt_pyrLevels() const {
		return pipeConf.getVxOt_pyrLevels();
	}
	int pipe_getVxOt_lkNumIters() const {
		return pipeConf.getVxOt_lkNumIters();
	}
	int pipe_getVxOt_lkWinSize() const {
		return pipeConf.getVxOt_lkWinSize();
	}
	int pipe_getVxOt_maxCorners() const {
		return pipeConf.getVxOt_maxCorners();
	}
	int pipe_getVxOt_detectorCellSize() const {
		return pipeConf.getVxOt_detectorCellSize();
	}
	int pipe_getVxOt_fastType() const {
		return pipeConf.getVxOt_fastType();
	}
	int pipe_getVxOt_fastThreshold() const {
		return pipeConf.getVxOt_fastThreshold();
	}
	float pipe_getVxOt_harrisK() const {
		return pipeConf.getVxOt_harrisK();
	}
	float pipe_getVxOt_harrisThreshold() const {
		return pipeConf.getVxOt_harrisThreshold();
	}
	int pipe_getVxOt_maxCornersInCell() const {
		return pipeConf.getVxOt_maxCornersInCell();
	}
	int pipe_getVxOt_xNumOfCells() const {
		return pipeConf.getVxOt_xNumOfCells();
	}
	int pipe_getVxOt_yNumOfCells() const {
		return pipeConf.getVxOt_yNumOfCells();
	}
	bool pipe_getVxOt_useFastDetector() const {
		return pipeConf.getVxOt_useFastDetector();
	}

#ifdef DEBUG_BUILD
	const string& dbg_getFilePipeFilePath() const
	{
		return dbgConf.getFilePipeFilePath();
	}
	bool dbg_getEnableTracking() const {
		return dbgConf.getEnableTracking();
	}
	const pair<pair<int, int>, pair<int,int>>& dbg_getVvxOt_preDefinedRectangle() const {
		return dbgConf.getVvxOt_preDefinedRectangle();
	}
#endif
};

}
/* namespace va */

#endif /* CONFIGURATION_H_ */
