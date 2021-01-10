/*
* Copyright (c) 2018-present, aliminabc@gmail.com.
*
* This source code is licensed under the MIT license found in the
* LICENSE file in the root directory of this source tree.
*/

#ifndef FK_FRAMEWORK_FKSESSIONEXECUTOR_H
#define FK_FRAMEWORK_FKSESSIONEXECUTOR_H

#include "FkObject.h"
#include "FkSession.h"
#include "FkProtocol.h"

FK_CLASS FkSessionExecutor FK_EXTEND FkObject {
public:
    FkSessionExecutor();

    virtual ~FkSessionExecutor();

    FkResult send(std::shared_ptr<FkSession> session, std::shared_ptr<FkProtocol> protocol);

    template<class T, typename... Args>
    FkResult quickSend(Args... chain) {
        auto session = FkSession::with<T>(std::make_shared<T>());
        auto ll = initializer_list<std::shared_ptr<FkQuark>>{chain...};
        for (auto it = ll.begin(); it != ll.end(); ++it) {
            auto ret = session->connectTo(*it);
            if (FK_OK != ret) {
                return ret;
            }
        }
        auto ret = session->open();
        if (FK_OK != ret) {
            return ret;
        }
        ret = this->send(session, std::make_shared<T>());
        if (FK_OK != ret) {
            return ret;
        }
        return session->close();
    }

private:
    FkSessionExecutor(const FkSessionExecutor &o) : FkObject() {};
};


#endif //FK_FRAMEWORK_FKSESSIONEXECUTOR_H
