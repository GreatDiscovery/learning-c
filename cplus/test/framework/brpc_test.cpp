//
// Created by 贾芸 on 2024/7/23.
//

#include "brpc_test.h"
#include <brpc/channel.h>
#include <brpc/controller.h>
#include <butil/logging.h>

TEST(brpc_test, 测试brpc_http调用) {
    brpc::Channel channel;
    brpc::ChannelOptions options;
    options.protocol = brpc::PROTOCOL_HTTP;
    options.connection_type = "single";
    options.timeout_ms = 1000;
    options.max_retry = 3;

    if (channel.Init("www.example.com:80", &options) != 0) {
        LOG(ERROR) << "Fail to initialize channel";
        return;
    }

    brpc::Controller cntl;
    cntl.http_request().uri() = "http://www.example.com";
    channel.CallMethod(NULL, &cntl, NULL, NULL, NULL);

    if (cntl.Failed()) {
        LOG(ERROR) << "Fail to send request, " << cntl.ErrorText();
    } else {
        LOG(INFO) << "Response: " << cntl.response_attachment().to_string();
    }
}