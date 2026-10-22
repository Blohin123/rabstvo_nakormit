#include "ServerForm.h"
#include <msclr/marshal_cppstd.h>
#include <vector>
#include <thread>
#include <chrono>


struct ClientInfo {
    std::unique_ptr<Socket> client;
    std::string ip;
};

System::Void MessengerAloha::ServerForm::shutdown_ui_update() {
    labelStatus->Text = "STATUS: OFFLINE";
    buttonRun->Text = "START MAX SERVER";

};

System::Void MessengerAloha::ServerForm::server_run(Object^ portSys) {
    WsaInitializer wsa;
    int port = (int)(portSys);
    ServerSocket serverSocket(port);

    System::String^ statusMsg =
        "STATUS: ONLINE (port: " + portSys->ToString() + ")";
    this->Invoke(
        gcnew status_update(this, &ServerForm::label_update),
        statusMsg
    );

    std::vector<ClientInfo> clients;

    while (is_online) {
        fd_set readSet;
        FD_ZERO(&readSet);
        FD_SET(serverSocket.getHandle(), &readSet);
        SOCKET maxFd = serverSocket.getHandle();

        for (const auto& client : clients) {
            SOCKET handle = client.client->getHandle();
            FD_SET(handle, &readSet);
            if (handle > maxFd)
                maxFd = handle;
        }

        timeval timeout;
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int activity = select(0, &readSet, nullptr, nullptr, &timeout);
        if (activity < 0)
            throw NetworkException("select failed");

        if (FD_ISSET(serverSocket.getHandle(), &readSet)) {
            auto client = serverSocket.accept();
            std::string ip = client->getPeerIp();
            clients.push_back({ std::move(client), ip });

            System::String^ logMessage =
                "Connected from: " +
                msclr::interop::marshal_as<System::String^>(ip);

            this->Invoke(
                gcnew log_update(this, &ServerForm::addLineLog),
                logMessage,
                System::Drawing::Color::Maroon
            );
        }

        for (size_t i = 0; i < clients.size();) {
            if (!FD_ISSET(clients[i].client->getHandle(), &readSet)) {
                ++i;
                continue;
            }

            try {
                std::string message = clients[i].client->receive(2048);

                // Удаляем завершающие переводы строк, например в /shutdown\r\n
                while (!message.empty() &&
                    (message.back() == '\n' || message.back() == '\r')) {
                    message.pop_back();
                }

                if (message == "/shutdown") {
                    const std::string notice = "Server shutting down";

                    // Уведомляем подключённых клиентов
                    for (const auto& client : clients) {
                        try {
                            client.client->send(notice);
                        }
                        catch (...) {
                            // Клиент мог отключиться во время рассылки
                        }
                    }

                    addLine(
                        "Server shutting down",
                        System::Drawing::Color::IndianRed
                    );

                    // Даём клиентам время получить уведомление
                    std::this_thread::sleep_for(std::chrono::seconds(3));

                    // ClientInfo уничтожает Socket через unique_ptr
                    clients.clear();
                    is_online = false;

                    // Не вызываем server_shutdown() из рабочего потока:
                    // он может ожидать завершения этого же потока.
                    if (!this->IsDisposed && this->IsHandleCreated) {
                        this->BeginInvoke(
                            gcnew System::Windows::Forms::MethodInvoker(
                                this,
                                &ServerForm::shutdown_ui_update
                            )
                        );
                    }

                    return;
                }

                // Обычная обработка сообщения
                std::string broadcastMsg =
                    "[" + clients[i].ip + "]: " + message + '\n';

                for (const auto& client : clients)
                    client.client->send(broadcastMsg);

                System::String^ messageLog =
                    msclr::interop::marshal_as<System::String^>(broadcastMsg);

                this->Invoke(
                    gcnew log_update(this, &ServerForm::addLineLog),
                    messageLog,
                    System::Drawing::Color::Black
                );
            }
            catch (const std::exception&) {
                System::String^ messageErr =
                    "Disconnected: " +
                    msclr::interop::marshal_as<System::String^>(
                        clients[i].ip + '\n'
                    );

                this->Invoke(
                    gcnew status_update(this, &ServerForm::label_update),
                    messageErr
                );

                clients.erase(clients.begin() + i);
                continue;
            }

            ++i;
        }
    }
}

System::Void MessengerAloha::ServerForm::server_shutdown() {
    if (!is_online) return;
    is_online = false;
    if (server != nullptr) {
        server->Join(2000);
        server->Abort(); //ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ ГРЕХ 
    }
    server = nullptr;
    addLine("Server shutdown", System::Drawing::Color::IndianRed);
    labelStatus->Text = "STATUS: OFFLINE";
    buttonRun->Text = "�����";
}

System::Void MessengerAloha::ServerForm::label_update(System::String^ text) {
    if (this->IsDisposed || !this->IsHandleCreated) return;
    labelStatus->Text = text;
}

System::Void MessengerAloha::ServerForm::addLine(const std::string& text, System::Drawing::Color color) {
    if (this->IsDisposed || !this->IsHandleCreated) return;
    System::String^ textSys = msclr::interop::marshal_as<System::String^>(text);
    if (this->InvokeRequired) {
        this->BeginInvoke(gcnew log_update(this, &ServerForm::addLineLog), textSys, color);
    }
    else addLineLog(textSys, color);
}

System::Void MessengerAloha::ServerForm::addLineLog(System::String^ line, System::Drawing::Color color) {
    logTextBox->SelectionStart = logTextBox->TextLength;
    logTextBox->SelectionLength = 0;
    logTextBox->SelectionColor = color;
    logTextBox->AppendText(line + System::Environment::NewLine);
    logTextBox->ScrollToCaret();
}

System::Void MessengerAloha::ServerForm::buttonRun_Click(System::Object^ sender, System::EventArgs^ e) {
    if (is_online) server_shutdown();
    else {
        is_online = true;
        buttonRun->Text = "�������";
        server = gcnew System::Threading::Thread(
            gcnew System::Threading::ParameterizedThreadStart(this, &ServerForm::server_run));
        server->IsBackground = true;
        server->Start(8080);
    }
}

