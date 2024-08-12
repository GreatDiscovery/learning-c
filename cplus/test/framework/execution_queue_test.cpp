//
// Created by 贾芸 on 2024/8/12.
//

#include "../basic.h"
#include <bthread/execution_queue.h>
#include <bthread/sys_futex.h>
#include <bthread/countdown_event.h>
#include <butil/time.h>
#include <butil/fast_rand.h>
#include <iostream>
#include <string>

using namespace std;

bool stopped = false;

class LongIntTask {
public:
    long value;
    bthread::CountdownEvent *event;

    LongIntTask(long v)
            : value(v), event(NULL) {}

    LongIntTask(long v, bthread::CountdownEvent *e)
            : value(v), event(e) {}

    LongIntTask() : value(0), event(NULL) {}

    ~LongIntTask() {
        if (event) {
            delete event;
        }
    }
};

int print(void *meta, bthread::TaskIterator<LongIntTask> &iter) {
    stopped = iter.is_queue_stopped();
    if (stopped) {
        for (; iter; ++iter) {
            if (iter->event) { iter->event->signal(); }
        }
        return 0;
    }
    int *result = (int *) meta;
    for (; iter; ++iter) {
        cout << "before result " << *result << endl;
        *result += iter->value;
        cout << "value:" << iter->value << endl;
        cout << "after result " << *result << endl;
        if (iter->event) { iter->event->signal(); }
    }
    return 0;
}


TEST(executino_queue_test, 测试队列累加器) {
    stopped = false;
    bthread::ExecutionQueueId<LongIntTask> queue_id = {0};
    bthread::ExecutionQueueOptions options;

    int status;
    bthread::execution_queue_start(&queue_id, &options, print, &status);
    cout << "started result=" << status << endl;

    for (size_t i = 0; i < 5; ++i) {
        bthread::execution_queue_execute(queue_id, i);
    }

    cout << "===============" << endl;
    // 停止线程池
    bthread::execution_queue_stop(queue_id);
    cout << "---------------" << endl;
    // 等待线程池任务全部完成后退出
    bthread::execution_queue_join(queue_id);
    cout << "result:" << status << endl;
}