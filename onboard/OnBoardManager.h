#ifndef ONBOARDMANAGER_H
#define ONBOARDMANAGER_H

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "../decoder/Decoder.h"
#include "../encoder/Encoder.h"
#include "../rf/RFPacket.h"
#include "../rf/RFReceiver.h"
#include "../rf/RFTransmitter.h"

#define NUM_DECODERS	7

class OnBoardManager
{
private:
	RFReceiver		m_Receiver;
	RFTransmitter	m_Transmitter;
	RFPacket		m_PacketTransmit;
	int				m_nLastDecode;
	Encoder*			m_encoder;

	Decoder* getEachDecoderToAttemptToDecodeThePacketAndGetDecoderThatManagedToDecodeThePacketIfItExists(RFPacket* packet);

public:
	OnBoardManager();

	void	setup();
	void	check();
	
	void	handle(NinjaPacket* pPacket);
};

#endif