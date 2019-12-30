#ifndef BOTFUNCTS_H_INCLUDED
#define BOTFUNCTS_H_INCLUDED


			std::string transformedMessageText(messageText);
			std::transform(transformedMessageText.begin(), transformedMessageText.end(), transformedMessageText.begin(), (int(*)(int))std::tolower);


			//check if someone needs help
			if (!transformedMessageText.substr(0, 5).compare("!help"))
			{
				if (messageText.size() > 5)
				{
					//find topic in list
					const std::string question(trimString(transformedMessageText.substr(5)));

					const Topic* const topic = findTopicInList(question);

					if (topic)
						sendMessage("PRIVMSG " + username + " :"  + question  + ": "+ topic->getAnswer() + "\r\n");
					else
						sendMessage("PRIVMSG " + username + " :" + question + ": Sorry, i have no answer to this question.\r\n");
				}
				else sendMessage("PRIVMSG " + username + " :Please be specific. e.g. !help shaders\r\n");
			}

			else if (!transformedMessageText.substr(0, 5).compare("!edit"))
			{
				if (messageText.size() > 5)
				{
					unsigned int count = 0, k = 0;
					while (messageText[k] != '\0')
					{
						if (messageText[k] == '|')
							++count;
						++k;
					}

					if (count == 4)
					{
						//extract topic
						const size_t pos = transformedMessageText.find("|");
						const size_t length = transformedMessageText.find("|", pos+1) - pos -1;
						const std::string question(transformedMessageText.substr(pos+1, length));

						//if topic is epty or not present, return
						if (!trimString(question).compare(""))
						{
							//empty topic
							sendMessage("PRIVMSG " + username + " :Usage: !edit |topic| |new answer|\r\n");
							return;
						}
						else if (findTopicInList(question) == 0)
						{
							sendMessage("PRIVMSG " + username + " :There is no such a topic. Please use the !fill command to create a new one\r\n");
							return;
						}

						//extract answer
						const size_t pos2 = transformedMessageText.find("|", transformedMessageText.find(question) + length + 1);
						const size_t length2 = transformedMessageText.find("|", pos2+1)- pos2 - 1;
						const std::string newAnswer(messageText.substr(pos2+1, length2));

						if (!trimString(newAnswer).compare(""))
						{
							sendMessage("PRIVMSG " + username + " :Usage: !edit |topic| |new answer|\r\n");
							return;
						}

						Topic* const topic = findTopicInList(question);
						topic->edit(newAnswer);
					}
					else sendMessage("PRIVMSG " + username + " :Usage: !edit |topic| |new answer|\r\n");
				}
				else sendMessage("PRIVMSG " + username + " :Usage: !edit |topic| |answer|\r\n");
			}

			//check if someone wants to add a topic
			else if (!transformedMessageText.substr(0, 5).compare("!fill"))
			{
				if (messageText.size() > 5)
				{
					unsigned int count = 0, k = 0;
					while (messageText[k] != '\0')
					{
						if (messageText[k] == '|')
							++count;
						++k;
					}

					if (count == 4)
					{
						//extract topic
						const size_t pos = transformedMessageText.find("|");
						const size_t length = transformedMessageText.find("|", pos+1) - pos -1;
						const std::string topic(trimString(transformedMessageText.substr(pos+1, length)));


						//if topic is empty or already there, return

						if (!topic.compare(""))
						{
							//empty topic
							sendMessage("PRIVMSG " + username + " :Usage: !fill |topic| |answer|\r\n");
							return;
						}
						else if (findTopicInList(topic))
						{
							sendMessage("PRIVMSG " + username + " :There already is such a topic.\r\n");
							return;
						}

						//extract answer
						const size_t pos2 = transformedMessageText.find("|", transformedMessageText.find(topic) + length + 1);
						const size_t length2 = transformedMessageText.find("|", pos2+1)- pos2 - 1;
						const std::string answer(messageText.substr(pos2+1, length2));

						if (!trimString(answer).compare(""))
						{
							sendMessage("PRIVMSG " + username + " :Usage: !fill |topic| |answer|\r\n");
							return;
						}

						createTopic(topic, answer);
					}
					else sendMessage("PRIVMSG " + username + " :Usage: !fill |topic| |answer|\r\n");
				}
				else sendMessage("PRIVMSG " + username + " :Usage: !fill \"topic\" \"answer\"\r\n");
			}

			else if (!transformedMessageText.substr(0, 6).compare("!erase"))
			{
				if (messageText.size() > 6)
				{
					const std::string question(trimString(transformedMessageText.substr(6)));

					if (findTopicInList(question))
						deleteTopic(question);
					else
						sendMessage("PRIVMSG " + username + " :There is no such a topic!\r\n");
				}
				else
				{
					sendMessage("PRIVMSG " + username + " :Usage: !erase topic\r\n");
				}
			}

			else if (!transformedMessageText.substr(0, 5).compare("!joke"))
			{
				const std::string joke("A very drunk man comes out of the bar and sees another very drunk man. He looks up in the sky and says, \"Is that the sun or the moon?\" The other drunk man answers, \"I don't know. I'm a stranger here myself.\"");
				sendMessage("PRIVMSG " + username + " :" + joke + "\r\n");
			}

			else if (!transformedMessageText.substr(0, 6).compare("!write"))
			{
				writeTopics();
			}
		}


		else if (!transformedIrcCommand.compare("join"))
		{

		    std::string username="tecan";
			std::cout << prefix << " " << message << " " << parameters << std::endl;

						std::string joinMessage = prefix.substr(0, prefix.find('!'));
						joinMessage += " has joined ";
						joinMessage += parameters.substr(parameters.find(':')+1);
					//	sendMessage("PRIVMSG " + username + " :Hello " + joinMessage + "\r\n");
						std::cout << joinMessage << std::endl;
						gui->addLine(irr::core::stringw(L""), joinMessage.c_str());
		}
		else if (!transformedIrcCommand.compare("part"))
		{

			std::string partMessage = prefix.substr(0, prefix.find('!'));
			partMessage += " has left ";
			partMessage += parameters.substr(parameters.find(':')+1);
            gui->addLine(irr::core::stringw(L""), partMessage.c_str());
			std::cout << partMessage << std::endl;

		}
		else if (!transformedIrcCommand.compare("quit"))
		{

			std::string quitMessage = prefix.substr(0, prefix.find('!'));
			quitMessage += " has quit";

			std::cout << quitMessage << std::endl;

		}
		else
		{
			std::cout << prefix << " " << message << " " << parameters << std::endl;
		}
	}
	else
	{
		//we recieved a ping, send pong back
		sendMessage("PONG " + message.substr(5) + "\r\n");
	}
#endif // BOTFUNCTS_H_INCLUDED
