#pragma once
#include <string>
#include <memory>
#include <sstream>
#include <boost/asio.hpp>
#include <map>
#include "question.h"
#include "message.h"
#include "publish_service.h"
#include "struct.h"
#include "recordSRV.h"
#include "traffic_monitor.h"

enum PublishState
{
	Announce1,
	Announce2,
	Announce3,
	Ready,
	Unpublish2,
	Unpublish3,
};

class PublishContext
{
public:
	PublishedService service_;
	PublishState state_;
	int countdown_;
	Message announce_packet_;
	Message unpublish_packet_;
	std::chrono::steady_clock::time_point elapsed_start_;
	double time_to_live_seconds_ = 4500.0;

	PublishContext(PublishedService service, PublishState state, int countdown,
		Message announce_packet, Message unpublish_packet, double time_to_live_seconds)
		: service_(service), state_(state), countdown_(countdown), 
		announce_packet_(announce_packet), unpublish_packet_(unpublish_packet),
		elapsed_start_(std::chrono::steady_clock::now()) {
	}

	// 重启计时器
	void RestartElapsed() {
		elapsed_start_ = std::chrono::steady_clock::now();
	}

	// 获取已用秒数
	double elapsedSeconds() const {
		auto now = std::chrono::steady_clock::now();
		return std::chrono::duration<double>(now - elapsed_start_).count();
	}
};

class Publisher {
public:
	std::string localQualify(std::string short_name) {
		if (short_name.size() < 7 || short_name.substr(short_name.size() - 7) != ".local.")
			return short_name + ".local.";
		return short_name;
	}
	Message makeClaimPacket(PublishedService service, boost::asio::ip::address_v4 ip) {
		Message message;
		std::string fq_long_name = service.long_name_ + "." + service.service_type_;
		std::string local_short_name = localQualify(service.short_name_);
		message.questions_.push_back(std::make_shared<Question>(fq_long_name, QType::ANY, QClass::QIN));
		message.questions_.push_back(std::make_shared<Question>(local_short_name, QType::ANY, QClass::QIN));

		RecordSRV srv(0, 0, service.port_, local_short_name);
		message.authorities_.push_back(std::make_shared<RR>(fq_long_name, short_time_to_live_, srv));

		// TODO
		RecordA arec(ip.to_bytes());
		message.authorities_.push_back(std::make_shared<RR>(local_short_name, short_time_to_live_, arec));
		return message;
	}

	Message makeAnnouncePacket(PublishedService service, boost::asio::ip::address_v4 ip) {
		Message message;
		message.header.setQR(true);
		message.header.setAA(true);

		std::string fq_long_name = service.long_name_ + "." + service.service_type_;
		std::string local_short_name = localQualify(service.short_name_);

		RecordSRV srv(0, 0, service.port_, local_short_name);
		message.answers_.push_back(std::make_shared<RR>(fq_long_name, short_time_to_live_, srv));
		message.answers_.back()->rrclass_ = static_cast<DnsClass>(0x8001);

		RecordTXT txt(service.txt_record_);
		message.answers_.push_back(std::make_shared<RR>(fq_long_name, long_time_to_live_, txt));
		message.answers_.back()->rrclass_ = static_cast<DnsClass>(0x8001);

		RecordPTR ptr1(service.service_type_);
		message.answers_.push_back(std::make_shared<RR>("_services._dns-sd._udp.local.", long_time_to_live_, ptr1));

		RecordPTR ptr2(fq_long_name);
		message.answers_.push_back(std::make_shared<RR>(service.service_type_, long_time_to_live_, ptr1));

		RecordA argc(ip.to_bytes());
		message.answers_.push_back(std::make_shared<RR>(local_short_name, short_time_to_live_, argc));
		message.answers_.back()->rrclass_ = static_cast<DnsClass>(0x8001);

		std::string arpa_ip_name = arpaIpName(ip);
		RecordPTR ptr3(local_short_name);
		message.answers_.push_back(std::make_shared<RR>(arpa_ip_name, short_time_to_live_, ptr3));
		message.answers_.back()->rrclass_ = static_cast<DnsClass>(0x8001);

	}

	Message makeUnpublishPacket(PublishedService service)
	{
		std::string fq_long_name = service.long_name_ + "." + service.service_type_;

		Message message;
		message.header.setQR(true);
		message.header.setAA(true);

		RecordPTR ptr(fq_long_name);
		message.answers_.push_back(std::make_shared<RR>(service.service_type_, 0, ptr));

		return message;
	}


	std::string arpaIpName(boost::asio::ip::address_v4 ip) {
		auto bytes = ip.to_bytes();
		return std::to_string(bytes[3]) + "." +
			std::to_string(bytes[2]) + "." +
			std::to_string(bytes[1]) + "." +
			std::to_string(bytes[0]) + ".in-addr.arpa.";
	}

	 bool publish(PublishedService service)
	{
		if (closing)
			return false;

		boost::asio::ip::address server_ip_address = traffic_monitor_.getServerIPAddress();
		Message announce_packet = makeAnnouncePacket(service, server_ip_address.to_v4());
		Message unpublish_packet = makeUnpublishPacket(service);

		PublishContext context(service, PublishState::Announce1, 2, announce_packet, 
						unpublish_packet, short_time_to_live_);


		Message claim = makeClaimPacket(context.service_, server_ip_address.to_v4());
		traffic_monitor_.broadcast(claim);

		{
			std::unique_lock<std::mutex> lock(mtx_);
			table_[service.long_name_] = context;
		}

		return true;
	}

private:
	const unsigned int long_time_to_live_ = 30;
	const unsigned int short_time_to_live_ = 30;
	std::map<std::string, PublishContext> table_;
	std::mutex mtx_;
	bool closing = false;
	TrafficMonitor traffic_monitor_;
};