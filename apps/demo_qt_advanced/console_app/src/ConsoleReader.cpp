/**
********************************************************************************
\file   ConsoleReader.cpp

\brief  Implementation of the console input reader thread.
*******************************************************************************/
#if (TARGET_SYSTEM == _WIN32_)
#define _WINSOCKAPI_
#endif  // (TARGET_SYSTEM == _WIN32_)

#include <pcap.h>

#include "../include/ConsoleReader.h"
#include <iostream>
#include <fstream>
#include "user/processimage/ProcessImageParser.h"
#include "common/XmlParserException.h"

#ifdef CONFIG_USE_PCAP
extern "C" tOplkError selectPcapDevice(char *pDevName_p);
#endif // CONFIG_USE_PCAP

ConsoleReader::ConsoleReader()
{
	nodeId = 240;
	devName.reserve(128);
	xapFileName = "xap.xml";
	sdoReadData = new ULONG();
	sdoWriteData = new ULONG();
	*sdoWriteData = 50000;

	sdoReadTransferJob = new SdoTransferJob(1, 0x1006, 0x00, (void*) sdoReadData,
		sizeof(UINT32), kSdoTypeAsnd, kSdoAccessTypeRead);

	sdoWriteTransferJob = new SdoTransferJob(240, 0x1006, 0x00, (void*) sdoWriteData,
		sizeof(UINT32), kSdoTypeAsnd, kSdoAccessTypeWrite);

	inputChannelName = "CN1.M02.X20DO9322.DigitalOutput01";
	outputChannelName = "CN1.M01.X20DI9371.DigitalInput02";
}

ULONG ConsoleReader::GetSdoReadData()
{
	return *sdoReadData;
}

ULONG ConsoleReader::GetSdoWriteData()
{
	return *sdoWriteData;
}

void ConsoleReader::run()
{
	tOplkError oplkRet = kErrorOk;
	char	cKey = 0;
	BOOL	fExit = FALSE;

	ProcessImageParser *pi =  ProcessImageParser::NewInstance(ProcessImageParserType::QT_XML_PARSER);

	std::ifstream ifsXap(xapFileName.c_str());
	std::string xapData((std::istreambuf_iterator<char>(ifsXap)), std::istreambuf_iterator<char>());
	try
	{
		pi->Parse(xapData.c_str());
		// char* a = NULL;
		// pi->Parse(a);
	}
	catch(const XmlParserException& ex)
	{
		qDebug("An Exception has occured: %s", ex.whatException());
	}
	catch(const std::exception& ex)
	{
		qDebug("An Exception has occured: %s", ex.what());
	}

	ProcessImageIn& piIn = static_cast<ProcessImageIn&>(pi->GetProcessImage(Direction::PI_IN));
	ProcessImageOut& piOut = static_cast<ProcessImageOut&>(pi->GetProcessImage(Direction::PI_OUT));

	qDebug("Size-in: %d", piIn.GetSize());
	qDebug("Size-out: %d", piOut.GetSize());


#ifdef CONFIG_USE_PCAP
	oplkRet = selectPcapDevice(&devName[0]);
	if (oplkRet != kErrorOk)
	{
		qDebug("selectPcapDevice retCode %x", oplkRet);
	}
#else
	devName = "epl";
#endif

	oplkRet = OplkQtApi::InitStack(this->nodeId, devName);
	if (oplkRet != kErrorOk)
	{
		qDebug("InitStack retCode %x", oplkRet);
	}

	oplkRet = OplkQtApi::AllocateProcessImage(piIn, piOut);
	if (oplkRet != kErrorOk)
	{
		qDebug("AllocateProcessImage retCode %x", oplkRet);
	}

	oplkRet = OplkQtApi::StartStack();
	if (oplkRet != kErrorOk)
	{
		qDebug("StartStack retCode %x", oplkRet);
		fExit = TRUE;
	}

	bool stackStated = true;
	BYTE *piInDataPtr;
	BYTE *piOutDataPtr;
	while (!fExit)
	{
		if (console_kbhit())
		{
			cKey = (BYTE)console_getch();
			switch (cKey)
			{
				case 'N': //NMT Command
				case 'n':
				{
					oplkRet =  OplkQtApi::ExecuteNmtCommand(1, kNmtCmdResetNode); //tNmtCommand
					if (oplkRet != kErrorOk)
					{
					//	fExit = TRUE;
					}
					break;
				}
				case 'R':
				case 'r':
				{
					int methodIndex1 = receiver.metaObject()->indexOfMethod(QMetaObject::normalizedSignature("HandleSdoTransferFinished(const SdoTransferResult)").constData());
					qDebug("Loop Method index1: %d", methodIndex1);
					oplkRet =  OplkQtApi::TransferObject(*sdoReadTransferJob,
							   receiver, receiver.metaObject()->method(methodIndex1));
					qDebug("Ret sdo %x",oplkRet);

					break;
				}
				case 'W':
				case 'w':
				{
					int methodIndex1 = receiver.metaObject()->indexOfMethod(QMetaObject::normalizedSignature("HandleSdoTransferFinished(const SdoTransferResult)").constData());
					qDebug("Loop Method index1: %d", methodIndex1);
					oplkRet =  OplkQtApi::TransferObject(*sdoWriteTransferJob,
							   receiver, receiver.metaObject()->method(methodIndex1));
					qDebug("Ret sdo %x", oplkRet);
					break;
				}
				case 't':
				case 'T':
				{
					oplkRet = oplk_exchangeProcessImageOut();
					if (oplkRet != kErrorOk)
					{
						qDebug("ExchangeProcessImageOut retCode %x", oplkRet);
					}
					std::cout<< "\n ProcessImageIn - PReq:  ";
					piInDataPtr = (BYTE*) oplk_getProcessImageIn();
					for (UINT piloop = 0; piloop < piIn.GetSize(); piloop++)
					{
						if (piInDataPtr != NULL)
						{
							std::cout<< std::hex << (int)(*piInDataPtr);
						}
						piInDataPtr++;
					}
					std::cout<< "\n ProcessImageOut - PRes:  ";
					piOutDataPtr = (BYTE*) oplk_getProcessImageOut();
					for (UINT piloop = 0; piloop < piOut.GetSize(); piloop++)
					{
						if (piOutDataPtr != NULL)
						{
							std::cout<< std::hex << (int)(*piOutDataPtr);
						}
						piOutDataPtr++;
					}

					oplkRet = oplk_exchangeProcessImageIn();
					if (oplkRet != kErrorOk)
					{
						qDebug("ExchangeProcessImageIn retCode %x", oplkRet);
					}
					break;
				}
				case 'i':
				case 'I':
				{
					try
					{
						const BYTE value = 0xFF;
						oplkRet = oplk_exchangeProcessImageOut();
						if (oplkRet != kErrorOk)
						{
							qDebug("ExchangeProcessImageOut retCode %x", oplkRet);
						}
						std::vector<BYTE> val;
						const Channel channelObj = piIn.GetChannel(inputChannelName);
						val.reserve(channelObj.GetBitSize());
						val.push_back(value);

						//piIn.SetRawValue(inputChannelName, val);
						piIn.SetRawData(val,0,0);

						std::cout<< "\n ProcessImageIn - PReq:  ";
						piInDataPtr = (BYTE*) oplk_getProcessImageIn();
						for (UINT i = 0; i < piIn.GetSize(); i++)
						{
							if (piInDataPtr != NULL)
							{
								std::cout<< std::hex << (int)(*piInDataPtr);
							}
							piInDataPtr++;
						}
						oplkRet = oplk_exchangeProcessImageIn();
						if (oplkRet != kErrorOk)
						{
							qDebug("ExchangeProcessImageIn retCode %x", oplkRet);
						}
					}
					catch(std::out_of_range& ex)
					{
						std::cout << "An Exception has occured: " << ex.what();
					}

					break;
				}
				case 'o':
				case 'O':
				{
					try
					{
						oplkRet = oplk_exchangeProcessImageOut();
						if (oplkRet != kErrorOk)
						{
							qDebug("ExchangeProcessImageOut retCode %x", oplkRet);
						}
						//std::vector<BYTE> outVal = piOut.GetRawValue(outputChannelName);
						std::vector<BYTE> outVal = piOut.GetRawData(16,2,0);
						std::cout<< "\nPI-Out Val: ";
						for (std::vector<BYTE>::const_iterator it = outVal.begin();
								it != outVal.end(); it++)
						{
							//BYTE ch = *it;
							std::cout << std::hex << (int)(*it);
						}
						std::cout << std::endl;
						oplkRet = oplk_exchangeProcessImageIn();
						if (oplkRet != kErrorOk)
						{
							qDebug("ExchangeProcessImageIn retCode %x", oplkRet);
						}
					}
					catch(std::out_of_range& ex)
					{
						std::cout << "An Exception has occured: " << ex.what();
					}
					break;
				}
				case 's':
				case 'S':
				{
					if(stackStated)
					{
						oplkRet = OplkQtApi::StopStack();
						if (oplkRet != kErrorOk)
						{
							qDebug("StartStack retCode %x", oplkRet);
						}
						stackStated = false;
					}
					else
					{
						oplkRet = OplkQtApi::InitStack(this->nodeId, devName);
						if (oplkRet != kErrorOk)
						{
							qDebug("InitStack retCode %x", oplkRet);
						}
						oplkRet = OplkQtApi::AllocateProcessImage(piIn, piOut);
						if (oplkRet != kErrorOk)
						{
							qDebug("AllocateProcessImage retCode %x", oplkRet);
						}
						oplkRet = OplkQtApi::StartStack();
						if (oplkRet != kErrorOk)
						{
							qDebug("StartStack retCode %x", oplkRet);
						}
						stackStated = true;
					}
					break;
				}
				case 0x1B:
					fExit = TRUE;
					break;

				default:
					break;
			}
			cKey = 0;
		}

		if (oplk_checkKernelStack() == FALSE)
		{
			fExit = TRUE;
			qDebug("Kernel stack has gone! Exiting...\n");
		}
	}
	qDebug("Reader thread Exiting...!\n");
}

