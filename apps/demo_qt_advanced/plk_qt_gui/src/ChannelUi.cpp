#include "ChannelUi.h"
#include <QDebug>
// direction = true for output
// todo change as ChannelUI(Channel, QWidget *parent)
ChannelUi::ChannelUi(Channel channel, QWidget *parent) :
	QWidget(parent),
	channel(channel)
{
	ui.setupUi(this);
	this->ui.channelName->setText(QString::fromStdString(this->channel.GetName()));

	// Hide force check box and forcevalue text box for output PI
	if (this->channel.GetDirection() == Direction::PI_OUT)
	{
		this->ui.force->hide();
		this->ui.forceValue->hide();
	}
	this->setToolTip(QString("Size = %1  ByteOffset = 0x%2  BitOffset = 0x%3")
					 .arg(this->channel.GetBitSize())
					 .arg(this->channel.GetByteOffset())
					 .arg(this->channel.GetBitOffset()));
	if ((this->channel.GetBitSize() % 8) == 0)
	{
		QString inputMask;
		for (uint i = 0; i < (this->channel.GetBitSize() / 4); ++i)
		{
			inputMask.append("H");
		}
		this->ui.forceValue->setInputMask(inputMask);
	}
	else
	{
		this->ui.forceValue->setMaxLength(1);
		this->ui.forceValue->setInputMask("H");
	}
}

ChannelUi::~ChannelUi()
{
	delete this->ui.channelName;
	delete this->ui.check;
	delete this->ui.currentValue;
	delete this->ui.force;
	delete this->ui.horizontalLayout;
}

void ChannelUi::on_check_stateChanged(int arg1)
{

}

void ChannelUi::on_force_stateChanged(int arg1)
{

}

void ChannelUi::UpdateSelectCheckBox(Qt::CheckState forceState)
{
	this->ui.check->setChecked(forceState);
}

Qt::CheckState ChannelUi::GetSelectCheckBoxState()
{
	return this->ui.check->checkState();
}

void ChannelUi::SetCurrentValue(QString setStr)
{
	this->ui.currentValue->setText(setStr);
}

QString ChannelUi::GetCurrentValue()
{
	return this->ui.currentValue->text();
}

QString ChannelUi::GetForceValue()
{
	return this->ui.forceValue->text();
}

void ChannelUi::UpdateForceCheckBox(Qt::CheckState forceState)
{
	this->ui.force->setChecked(forceState);
}

Qt::CheckState ChannelUi::GetForceCheckBoxState()
{
	return this->ui.force->checkState();
}

void ChannelUi::UpdateInputChannelCurrentValue(ProcessImageIn *in)
{
	try
	{
		std::vector<BYTE> value = in->GetRawData(this->channel.GetBitSize(),
												this->channel.GetByteOffset(),
												this->channel.GetBitOffset());
		// std::vector<BYTE> value = in->GetRawValue(this->channel.GetName());
		QString string;
		for (std::vector<BYTE>::reverse_iterator it = value.rbegin();
				it != value.rend(); ++it )
		{
			string.append(QString("%1").arg(*it, 0, 16)).rightJustified(2, '0');
		}
//		for (std::vector<BYTE>::const_iterator it = value.begin();
//				it != value.end(); ++it)
//		{
//			string.append(QString("%1").arg(*it, 0, 16)).rightJustified(2, '0');
//		}
		this->SetCurrentValue(string);

		if (this->ui.force->checkState() == Qt::Checked)
		{
			const QString forceValue = this->GetForceValue();
			const qlonglong forc = forceValue.toLongLong(0, 16);
			// qDebug("%u", forc);
			in->SetRawValue(this->channel.GetName(), (const void*) &forc, this->channel.GetBitSize());
		}
	}
	catch(const std::exception& ex)
	{
		qDebug("An Exception has occured: %s", ex.what());
	}
}

void ChannelUi::UpdateOutputChannelCurrentValue(ProcessImageOut *out)
{
	try
	{
		std::vector<BYTE> value = out->GetRawData(this->channel.GetBitSize(),
												this->channel.GetByteOffset(),
												this->channel.GetBitOffset());
		// std::vector<BYTE> value = out->GetRawValue(this->channel.GetName());
		QString string;
		for (std::vector<BYTE>::reverse_iterator it = value.rbegin();
				it != value.rend(); ++it )
		{
			string.append(QString("%1").arg(*it, 0, 16)).rightJustified(2, '0');
		}
//		for (std::vector<BYTE>::const_iterator it = value.begin();
//				it != value.end(); ++it)
//		{
//			string.append(QString("%1").arg(*it, 0, 16)).rightJustified(2, '0');
//		}
		this->SetCurrentValue(string);
	}
	catch(const std::exception& ex)
	{
		qDebug("An Exception has occured: %s", ex.what());
	}
	// TODO test.

//	qulonglong val;
//	out->GetRawValue(this->channel.GetName(), (void*) &val, this->channel.GetBitSize());
//	qDebug("%u", val);
//	this->SetCurrentValue(QString("0x%1").arg(val, 0, 10));
}
