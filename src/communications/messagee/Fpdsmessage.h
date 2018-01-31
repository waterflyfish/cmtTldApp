/*
 * FeaturePointsTrackedDebugStatusMessage.h
 *
 *  Created on: Mar 4, 2017
 *      Author: patrick
 */
#ifndef FPDSMESSAGE_H_
#define FPDSMESSAGE_H_



#include "StatusMessage.h"

namespace va {

class Fpdsmessage: public StatusMessage {
public:
	Fpdsmessage(string msgId, string loggerName, string targetId);
	virtual ~Fpdsmessage();

	float getError() const {
		return error;
	}

	void setError(float error) {
		this->error = error;
	}

	float getOrientation() const {
		return orientation;
	}

	void setOrientation(float orientation) {
		this->orientation = orientation;
	}

	float getScale() const {
		return scale;
	}

	void setScale(float scale) {
		this->scale = scale;
	}

	float getStrength() const {
		return strength;
	}

	void setStrength(float strength) {
		this->strength = strength;
	}

	int getTrackingStatus() const {
		return tracking_status;
	}

	void setTrackingStatus(int trackingStatus) {
		tracking_status = trackingStatus;
	}

	float getX() const {
		return x;
	}

	void setX(float x) {
		this->x = x;
	}

	float getY() const {
		return y;
	}

	void setY(float y) {
		this->y = y;
	}

	int getSerial() const {
		return serial;
	}

	void setSerial(int serial) {
		this->serial = serial;
	}

	private:
	int serial;
	float 	error;
	float 	orientation;
	float 	scale;
	float 	strength;
	int 	tracking_status ;
	float 	x;
	float 	y;
};

} /* namespace va */

#endif

