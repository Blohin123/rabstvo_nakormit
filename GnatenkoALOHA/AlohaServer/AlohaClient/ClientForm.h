#pragma once
#include <string>
#include <SocketWrapper/Client.h>

namespace AlohaClient {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	public delegate void log_update(System::String^ text, System::Drawing::Color color); // Добавлено имя переменной color или просто оставлен тип, но для делегатов C++/CLI лучше указывать полную сигнатуру
	/// <summary>
	/// Форма для ClientForm
	/// </summary>
	public ref class ClientForm : public System::Windows::Forms::Form
	{
	public:
		ClientForm(void)
		{
			InitializeComponent();
			worker = nullptr;
			client = nullptr;
			is_connected = false;
		}

	protected:
		/// <summary>
		/// Освободить все используемые ресурсы.
		/// </summary>
		~ClientForm()
		{
			if (components)
			{
				delete components;
			}
			client_shutdown();
		}
	private: System::Windows::Forms::TextBox^ textBoxSend;
	private: System::Windows::Forms::Button^ buttonSendMsg;
	private: System::Windows::Forms::RichTextBox^ richTextChat;
	private: System::Windows::Forms::GroupBox^ groupBoxConnect;
	private: System::Windows::Forms::Label^ labelServerIp;
	private: System::Windows::Forms::TextBox^ textBoxPort;
	private: System::Windows::Forms::TextBox^ textBoxServerIp;
	private: System::Windows::Forms::Button^ buttonConnect;
	private: System::Windows::Forms::Label^ labelPort;

	private: System::Threading::Thread^ worker;
	private: ClientSocket* client;
	private: bool is_connected;
	protected:

	protected:


	private:
		/// <summary>
		/// Обязательная переменная конструктора.
		/// </summary>
		System::ComponentModel::Container^ components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Требуемый метод для поддержки конструктора — не изменяйте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->textBoxSend = (gcnew System::Windows::Forms::TextBox());
			this->buttonSendMsg = (gcnew System::Windows::Forms::Button());
			this->richTextChat = (gcnew System::Windows::Forms::RichTextBox());
			this->groupBoxConnect = (gcnew System::Windows::Forms::GroupBox());
			this->buttonConnect = (gcnew System::Windows::Forms::Button());
			this->labelPort = (gcnew System::Windows::Forms::Label());
			this->labelServerIp = (gcnew System::Windows::Forms::Label());
			this->textBoxPort = (gcnew System::Windows::Forms::TextBox());
			this->textBoxServerIp = (gcnew System::Windows::Forms::TextBox());
			this->groupBoxConnect->SuspendLayout();
			this->SuspendLayout();
			// 
			// textBoxSend
			// 
			this->textBoxSend->Location = System::Drawing::Point(32, 462);
			this->textBoxSend->Name = L"textBoxSend";
			this->textBoxSend->Size = System::Drawing::Size(195, 20);
			this->textBoxSend->TabIndex = 0;
			// 
			// buttonSendMsg
			// 
			this->buttonSendMsg->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->buttonSendMsg->Location = System::Drawing::Point(233, 462);
			this->buttonSendMsg->Name = L"buttonSendMsg";
			this->buttonSendMsg->Size = System::Drawing::Size(110, 38);
			this->buttonSendMsg->TabIndex = 1;
			this->buttonSendMsg->Text = L"SEND";
			this->buttonSendMsg->UseVisualStyleBackColor = true;
			this->buttonSendMsg->Click += gcnew System::EventHandler(this, &ClientForm::buttonSendMsg_Click); // Добавлена привязка события клика кнопки SEND, которая отсутствовала в исходном коде дизайнера
			// 
			// richTextChat
			// 
			this->richTextChat->Location = System::Drawing::Point(11, 26);
			this->richTextChat->Name = L"richTextChat";
			this->richTextChat->Size = System::Drawing::Size(338, 430);
			this->richTextChat->TabIndex = 2;
			this->richTextChat->Text = L"";
			// 
			// groupBoxConnect
			// 
			this->groupBoxConnect->Controls->Add(this->buttonConnect);
			this->groupBoxConnect->Controls->Add(this->labelPort);
			this->groupBoxConnect->Controls->Add(this->labelServerIp);
			this->groupBoxConnect->Controls->Add(this->textBoxPort);
			this->groupBoxConnect->Controls->Add(this->textBoxServerIp);
			this->groupBoxConnect->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->groupBoxConnect->Location = System::Drawing::Point(396, 68);
			this->groupBoxConnect->Name = L"groupBoxConnect";
			this->groupBoxConnect->Size = System::Drawing::Size(271, 194);
			this->groupBoxConnect->TabIndex = 3;
			this->groupBoxConnect->TabStop = false;
			this->groupBoxConnect->Text = L"CONNECT";
			this->groupBoxConnect->Enter += gcnew System::EventHandler(this, &ClientForm::groupBoxConnect_Enter);
			// 
			// buttonConnect
			// 
			this->buttonConnect->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->buttonConnect->Location = System::Drawing::Point(143, 60);
			this->buttonConnect->Name = L"buttonConnect";
			this->buttonConnect->Size = System::Drawing::Size(110, 108);
			this->buttonConnect->TabIndex = 4;
			this->buttonConnect->Text = L"connect";
			this->buttonConnect->UseVisualStyleBackColor = true;
			this->buttonConnect->Click += gcnew System::EventHandler(this, &ClientForm::buttonConnect_Click);
			// 
			// labelPort
			// 
			this->labelPort->AutoSize = true;
			this->labelPort->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->labelPort->Location = System::Drawing::Point(25, 110);
			this->labelPort->Name = L"labelPort";
			this->labelPort->Size = System::Drawing::Size(78, 16);
			this->labelPort->TabIndex = 7;
			this->labelPort->Text = L"enter PORT";
			// 
			// labelServerIp
			// 
			this->labelServerIp->AutoSize = true;
			this->labelServerIp->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 9.75F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->labelServerIp->Location = System::Drawing::Point(25, 49);
			this->labelServerIp->Name = L"labelServerIp";
			this->labelServerIp->Size = System::Drawing::Size(55, 16);
			this->labelServerIp->TabIndex = 6;
			this->labelServerIp->Text = L"enter IP ";
			// 
			// textBoxPort
			// 
			this->textBoxPort->Location = System::Drawing::Point(18, 129);
			this->textBoxPort->Name = L"textBoxPort";
			this->textBoxPort->Size = System::Drawing::Size(119, 26);
			this->textBoxPort->TabIndex = 5;
			// 
			// textBoxServerIp
			// 
			this->textBoxServerIp->Location = System::Drawing::Point(18, 72);
			this->textBoxServerIp->Name = L"textBoxServerIp";
			this->textBoxServerIp->Size = System::Drawing::Size(119, 26);
			this->textBoxServerIp->TabIndex = 4;
			// 
			// ClientForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(701, 547);
			this->Controls->Add(this->groupBoxConnect);
			this->Controls->Add(this->richTextChat);
			this->Controls->Add(this->buttonSendMsg);
			this->Controls->Add(this->textBoxSend);
			this->Name = L"ClientForm";
			this->Text = L"ClientForm";
			this->Load += gcnew System::EventHandler(this, &ClientForm::ClientForm_Load);
			this->groupBoxConnect->ResumeLayout(false);
			this->groupBoxConnect->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void client_shutdown();
	private: System::Void client_run(System::Object^ obj);
	private: System::Void addLine(const std::string& text, System::Drawing::Color color);
	private: System::Void addLineLog(System::String^ line, System::Drawing::Color color);
	private: System::Void buttonConnect_Click(System::Object^ sender, System::EventArgs^ e);
	private: System::Void buttonSendMsg_Click(System::Object^ sender, System::EventArgs^ e); 
	private: System::Void groupBoxConnect_Enter(System::Object^ sender, System::EventArgs^ e) {
	};
	private: System::Void ClientForm_Load(System::Object^ sender, System::EventArgs^ e) {};
	}; 
}
