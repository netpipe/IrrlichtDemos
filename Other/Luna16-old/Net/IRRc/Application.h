#ifndef APPLICATION_H_
#define APPLICATION_H_

#include <vector>
#include <irrlicht.h>
#include "gui/Gui.h"
#include "Socket.h"

class Application : public irr::IEventReceiver
{

public:

	Application(irr::IrrlichtDevice *device2);

	~Application();

	bool init();

	void run() const;

	bool OnEvent(const irr::SEvent& event);
	irr::IrrlichtDevice* device;
    inline void registerIrrDevice(irr::IrrlichtDevice *device2) {
	this->device = device2;
}
private:


	Gui* gui;

	class Topic
	{

	public:

		Topic(const std::string& question, const std::string& answer) :
			question(question), answer(answer) { }

		void edit(const std::string& newAnswer)	{ this->answer = newAnswer; }

		inline const std::string& getQuestion() const { return this->question; }

		inline const std::string& getAnswer() const { return this->answer; }


	private:

		std::string question;

		std::string answer;
	};


	std::vector<Topic*> topics;

	///Reads the topics from file
	bool readTopics();

	///Writes the topics to file
	bool writeTopics() const;

	///Creates a new topic
	void createTopic(const std::string& question, const std::string& answer);

	///Delete topic
	void deleteTopic(const std::string& question);

	///Find topic by question
	Topic* const findTopicInList(const std::string& question) const;



	TCPSocket* socket;

	bool running;

	bool sendMessage(const std::string& message) const;

	static void receive(Application* const app);

	void handleIncommingMessages(std::string& message);



	static inline std::string trimString(const std::string& Src, const std::string& c = " \r\n")
	{
		const size_t p2 = Src.find_last_not_of(c);
		if (p2 == std::string::npos)
			return std::string();

		size_t p1 = Src.find_first_not_of(c);
		if (p1 == std::string::npos)
			p1 = 0;

		return Src.substr(p1, (p2-p1)+1);
	}
	/*
	static inline void replace_all(std::string& text, const std::string& fnd, const std::string& rep)
	{
		size_t pos = text.find(fnd);
		while(pos != std::string::npos)
		{
			text.replace(pos, fnd.length(), rep);
			pos = text.find(fnd, pos + rep.length());
		}
	}
	 */
};

#endif /* APPLICATION_H_ */
