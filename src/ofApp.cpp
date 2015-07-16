#include "ofApp.h"


#pragma region Falling
void Falling::setup()
{
	_Umbrella.setup(_fSize);
	_Giraffe.setup(_fSize);

	_AnimDropPos.setCurve(AnimCurve::LINEAR);
	_AnimDropPos.setDuration(ofRandom(cLIMIT_OF_FALLING_TIME.first, cLIMIT_OF_FALLING_TIME.second));

	_AnimSwing.setCurve(AnimCurve::EASE_IN_EASE_OUT);
	_AnimSwing.setDuration(ofRandom(cLIMIT_OF_SWING_TIME.first, cLIMIT_OF_SWING_TIME.second));
	_AnimSwing.setRepeatType(AnimRepeat::LOOP_BACK_AND_FORTH);
	
	this->play();
}

//--------------------------------------------------------------
void Falling::update(float fDelta)
{
	if(!_bAlive)
	{
		return;
	}
	_AnimDropPos.update(fDelta);
	_AnimSwing.update(fDelta);

	_Umbrella.update(fDelta);

	if(_AnimDropPos.hasFinishedAnimating() && _AnimDropPos.getPercentDone() == 1.0)
	{
		_bAlive = false;
	}
}

//--------------------------------------------------------------
void Falling::draw()
{
	if(!_bAlive)
	{
		return;
	}
	ofPushStyle();
	ofPushMatrix();
	ofTranslate(_AnimDropPos.getCurrentPosition());
	ofRotateZ(_AnimSwing.getCurrentValue());
	ofTranslate(0, _fSwingLength);
	{
		_Umbrella.draw(ofVec2f(0));

		float fSwingSpeed_ = _AnimSwing.getSpeedWithDirection();
		ofPushMatrix();
		ofTranslate(0, _fSize * 0.5);
		ofRotateZ(fSwingSpeed_ * -20);
		{
			_Giraffe.draw(ofVec2f(_fSize * 0.035, _fSize * 1.27));
		}
		ofPopMatrix();
	}
	ofPopMatrix();
	ofPopStyle();
}

//--------------------------------------------------------------
void Falling::play()
{
	
	_AnimDropPos.setPosition(ofVec2f(ofRandomWidth(), -(_fSwingLength * 1.3)));
	_AnimDropPos.animateTo(ofVec2f(ofRandomWidth(), ofGetHeight()));

	float fDegree_  = ofRandom(20, 45);
	_AnimSwing.animateFromTo(-fDegree_, fDegree_);
}
#pragma endregion

#pragma region ofApp
//--------------------------------------------------------------
void ofApp::setup()
{
	ofBackground(255);
	ofSetVerticalSync(true);

	_ShakeCanvas.setup();
	_ShakeCanvas.startShake();

	_bStart = false;
	_fAddTimer = ofRandom(cLIMIT_OF_CREATE_TIME.first, cLIMIT_OF_CREATE_TIME.second);

	_fTimer = ofGetElapsedTimef();
}

//--------------------------------------------------------------
void ofApp::update()
{
	float fDelta_ = ofGetElapsedTimef() - _fTimer;
	_fTimer += fDelta_;

	

	for(auto& Iter_ : _Falling)
	{
		Iter_.update(fDelta_);
	}

	//Remove the die elements
	auto Iter_ = _Falling.begin();
	while(Iter_ != _Falling.end())
	{
		if(!Iter_->getAlive())
		{
			Iter_ = _Falling.erase(Iter_);
		}
		else
		{
			Iter_++;
		}
	}

	//Update the shake canvas
	if( ofGetFrameNum() % 5 == 0)
	{
		_ShakeCanvas.update();		
	}

	//Add new filling
	if(_bStart)
	{
		_fAddTimer -= fDelta_;
		if(_fAddTimer <= 0.0)
		{
			_fAddTimer = ofRandom(cLIMIT_OF_CREATE_TIME.first, cLIMIT_OF_CREATE_TIME.second);
			this->addFalling();
		}
	}

	ofSetWindowTitle(ofToString(ofGetFrameRate()));
}

//--------------------------------------------------------------
void ofApp::draw()
{
	ofBackgroundGradient(ofColor(0, 69, 111), ofColor(248, 96, 14), OF_GRADIENT_LINEAR);
	_ShakeCanvas.begin();
	{
		for(auto& Iter_ : _Falling)
		{
			Iter_.draw();
		}
	}
	_ShakeCanvas.end();

	_ShakeCanvas.draw();
}

//--------------------------------------------------------------
void ofApp::addFalling()
{
	if(_Falling.size() < cMAX_NUM_OF_FALLING)
	{
		Falling newFalling_;
		newFalling_.setup();
	
		_Falling.push_back(newFalling_);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	switch(key)
	{
	case 'a':
		{
			this->addFalling();
		}
		break;
	case 's':
		{
			_bStart = !_bStart;
		}
		break;
	}
}
#pragma endregion

