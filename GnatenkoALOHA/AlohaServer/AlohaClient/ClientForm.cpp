#include "ClientForm.h"
#include <msclr/marshal_cppstd.h>

public ref class ConnectInfo : public System::Object {
public:
    System::String^ host;
    int serverPort;
    int clientPort;
    ConnectInfo(System::String^ host, int sPort, int cPort)
        : host(host), serverPort(sPort), clientPort(cPort) {
    }
};

System::Void AlohaClient::ClientForm::client_shutdown() {
    if (!is_connected) return;
    is_connected = false;
    if (worker != nullptr) {
        worker->Join(5000);
        worker->Abort(); //грех
    }
    worker = nullptr;
    delete client;
    client = nullptr;
}

System::Void AlohaClient::ClientForm::client_run(System::Object^ obj) {
    ConnectInfo^ info = (ConnectInfo^)obj;
    WsaInitializer wsa;
    client = new ClientSocket(info->clientPort);
    msclr::interop::marshal_context context;
    try {
        client->connect(context.marshal_as<std::string>(info->host), info->serverPort);
        System::String^ connectionMsg = "Подключен к серверу " + info->host + ":" + System::Convert::ToString(info->serverPort);
        System::Windows::Forms::MessageBox::Show(connectionMsg);
        this->Invoke(gcnew log_update(this, &ClientForm::addLineLog), connectionMsg, System::Drawing::Color::DarkMagenta);

    }
    catch (const std::exception& e) {
        System::Windows::Forms::MessageBox::Show("Connection failed: " + msclr::interop::marshal_as<System::String^>(e.what())
        );
        return;
    }
    try {
        while (is_connected) {
            System::String^ message = msclr::interop::marshal_as<System::String^>(client->receive(2048));
            this->Invoke(gcnew log_update(this, &ClientForm::addLineLog),
                message, System::Drawing::Color::Black
            );
        }
    }
    catch (const NetworkException& e) {
        System::Windows::Forms::MessageBox::Show(
            "Disconnected from server: "
            + msclr::interop::marshal_as<System::String^>(e.what()));
    }
    catch (const std::exception& e) {
        System::Windows::Forms::MessageBox::Show("Receiver error: " + msclr::interop::marshal_as<System::String^>(e.what()));
    }
} 

System::Void AlohaClient::ClientForm::addLine(const std::string& text, System::Drawing::Color color) { 
    if (this->IsDisposed || !this->IsHandleCreated) return;
    System::String^ sysText = msclr::interop::marshal_as<System::String^>(text);
    if (this->BeginInvoke(gcnew log_update(this, &ClientForm::addLineLog), sysText, color));
    else addLineLog(sysText, color);
}

System::Void AlohaClient::ClientForm::addLineLog(System::String^ line, System::Drawing::Color color) { 
    richTextChat->SelectionStart = richTextChat->TextLength;
    richTextChat->SelectionLength = 0;
    richTextChat->SelectionColor = color;
    richTextChat->AppendText(line + System::Environment::NewLine);
    richTextChat->ScrollToCaret();
}

System::Void AlohaClient::ClientForm::buttonConnect_Click(System::Object^ sender, System::EventArgs^ e) { 
    if (is_connected) client_shutdown();
    else {
        is_connected = true;
        buttonConnect->Text = "Quit";
        worker = gcnew System::Threading::Thread(
            gcnew System::Threading::ParameterizedThreadStart(this, &ClientForm::client_run));
        ConnectInfo^ info = gcnew ConnectInfo(
            textBoxServerIp->Text,
            System::Convert::ToInt32(textBoxPort->Text),
            8000
        );
        worker->IsBackground = true;
        worker->Start(info);
    }
}

System::Void AlohaClient::ClientForm::buttonSendMsg_Click(System::Object^ sender, System::EventArgs^ e) { 
    if (System::String::IsNullOrWhiteSpace(textBoxSend->Text)) return;
    msclr::interop::marshal_context context;
    std::string message = context.marshal_as <std::string>(textBoxSend->Text);
    client->send(message); 
}
