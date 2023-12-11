
#include <libircclient.h>
#include <irrlicht.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <stdarg.h>

#include "server.h"
#include "Application.h"

Application* g_app;

// #define DEBUG

#define IRC (irc_session_t*)irc_

/*
 * We store data in IRC session context.
 */
typedef struct
{
	char 	* channel;
	char 	* nick;

} irc_ctx_t;


std::vector<std::string> tokenize(const std::string & delim, const std::string & str)
{
	using namespace std;
	vector<std::string> tokens;

	size_t p0 = 0, p1 = std::string::npos;
	while (p0 != std::string::npos)
	{
		p1 = str.find_first_of(delim, p0);
		if (p1 != p0)
		{
			std::string token = str.substr(p0, p1 - p0);
			tokens.push_back(token);
		}
		p0 = str.find_first_not_of(delim, p1);
	}

	return tokens;
}


Server::Server(const ServerConfig &sc)
: config_(sc)
, irc_(0)
{
}

Server::~Server()
{
	irc_destroy_session(IRC);
}

const ServerConfig &Server::config() const
{
	return config_;
}


void addlog (const char * fmt, ...)
{
	FILE * fp;
	char buf[1024];
	va_list va_alist;

	va_start (va_alist, fmt);
#if defined (_WIN32)
	printf (buf, sizeof(buf), fmt, va_alist);
#else
	vsnprintf (buf, sizeof(buf), fmt, va_alist);
#endif
	va_end (va_alist);

	printf ("%s\n", buf);

	if ( (fp = fopen ("irctest.log", "ab")) != 0 )
	{
		fprintf (fp, "%s\n", buf);
		fclose (fp);
	}
}


void dump_event (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	char buf[512];
	int cnt;

	buf[0] = '\0';

	for ( cnt = 0; cnt < count; cnt++ )
	{
		if ( cnt )
			strcat (buf, "|");

		strcat (buf, params[cnt]);
	}


	addlog ("Event \"%s\", origin: \"%s\", params: %d [%s]", event, origin ? origin : "NULL", cnt, buf);
}

void event_leave(irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	dump_event(session, event, origin, params, count);
	irc_cmd_names(session, params[0]);

	//update GUI
	//irr::core::stringw who(origin);
	//who = who.subString(0, who.findFirst('!'));
	//g_app->gui->addLine(who + " leaved to ", irr::core::stringw(params[0]));

}

void event_join (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	dump_event (session, event, origin, params, count);
	irc_cmd_user_mode (session, "+i");
	//irc_cmd_msg (session, params[0], "Hi all");

	irc_cmd_names(session, params[0]);

	//update GUI
	irr::core::stringw who(origin);
	who = who.subString(0, who.findFirst('!'));
	g_app->gui->addLine(who + " joined to ", irr::core::stringw(params[0]));


}


void event_connect (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	//irc_ctx_t * ctx = (irc_ctx_t *) irc_get_ctx (session);
	dump_event (session, event, origin, params, count);

	//irc_cmd_join (session, ctx->channel, 0);

	g_app->gui->ShowJoinChannel();
}


void event_privmsg (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	dump_event (session, event, origin, params, count);

	printf ("'%s' said me (%s): %s\n",
		origin ? origin : "someone",
		params[0], params[1] );
}


void dcc_recv_callback (irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length)
{
	static int count = 1;
	char buf[12];

	switch (status)
	{
	case LIBIRC_ERR_CLOSED:
		printf ("DCC %d: chat closed\n", id);
		break;

	case 0:
		if ( !data )
		{
			printf ("DCC %d: chat connected\n", id);
			irc_dcc_msg	(session, id, "Hehe");
		}
		else
		{
			printf ("DCC %d: %s\n", id, data);
			sprintf (buf, "DCC [%d]: %d", id, count++);
			irc_dcc_msg	(session, id, buf);
		}
		break;

	default:
		printf ("DCC %d: error %s\n", id, irc_strerror(status));
		break;
	}
}


void dcc_file_recv_callback (irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length)
{
	if ( status == 0 && length == 0 )
	{
		printf ("File sent successfully\n");

		if ( ctx )
			fclose ((FILE*) ctx);
	}
	else if ( status )
	{
		printf ("File sent error: %d\n", status);

		if ( ctx )
			fclose ((FILE*) ctx);
	}
	else
	{
		if ( ctx )
			fwrite (data, 1, length, (FILE*) ctx);
		printf ("File sent progress: %d\n", length);
	}
}


void event_channel (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	char nickbuf[128];

	if ( count != 2 )
		return;

	printf ("'%s' said in channel %s: %s\n",
		origin ? origin : "someone",
		params[0], params[1] );

	//update GUI
	irr::core::stringw who(origin);
	who = who.subString(0, who.findFirst('!'));
	g_app->gui->addLine(who + " : ", irr::core::stringw(params[1]));

	if ( !origin )
		return;

	irc_target_get_nick (origin, nickbuf, sizeof(nickbuf));

	if ( !strcmp (params[1], "quit") )
		irc_cmd_quit (session, "of course, Master!");

	if ( !strcmp (params[1], "help") )
	{
		irc_cmd_msg (session, params[0], "quit, help, dcc chat, dcc send, ctcp");
	}

	if ( !strcmp (params[1], "ctcp") )
	{
		irc_cmd_ctcp_request (session, nickbuf, "PING 223");
		irc_cmd_ctcp_request (session, nickbuf, "FINGER");
		irc_cmd_ctcp_request (session, nickbuf, "VERSION");
		irc_cmd_ctcp_request (session, nickbuf, "TIME");
	}

	if ( !strcmp (params[1], "dcc chat") )
	{
		irc_dcc_t dccid;
		irc_dcc_chat (session, 0, nickbuf, dcc_recv_callback, &dccid);
		printf ("DCC chat ID: %d\n", dccid);
	}

	if ( !strcmp (params[1], "dcc send") )
	{
		irc_dcc_t dccid;
		irc_dcc_sendfile (session, 0, nickbuf, "irctest.c", dcc_file_recv_callback, &dccid);
		printf ("DCC send ID: %d\n", dccid);
	}

	if ( !strcmp (params[1], "topic") )
		irc_cmd_topic (session, params[0], 0);
	else if ( strstr (params[1], "topic ") == params[1] )
		irc_cmd_topic (session, params[0], params[1] + 6);

	if ( strstr (params[1], "mode ") == params[1] )
		irc_cmd_channel_mode (session, params[0], params[1] + 5);

	if ( strstr (params[1], "nick ") == params[1] )
		irc_cmd_nick (session, params[1] + 5);

	if ( strstr (params[1], "whois ") == params[1] )
		irc_cmd_whois (session, params[1] + 5);
}


void irc_event_dcc_chat (irc_session_t * session, const char * nick, const char * addr, irc_dcc_t dccid)
{
	printf ("DCC chat [%d] requested from '%s' (%s)\n", dccid, nick, addr);

	irc_dcc_accept (session, dccid, 0, dcc_recv_callback);
}


void irc_event_dcc_send (irc_session_t * session, const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid)
{
	FILE * fp;
	printf ("DCC send [%d] requested from '%s' (%s): %s (%lu bytes)\n", dccid, nick, addr, filename, size);

	if ( (fp = fopen ("file", "wb")) == 0 )
		abort();

	irc_dcc_accept (session, dccid, fp, dcc_file_recv_callback);
}

void event_numeric (irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count)
{
	char buf[24];
	sprintf (buf, "%d", event);
	dump_event(session, buf, origin, params, count);

	switch (event)
	{
	case 353: {
		std::string members(params[3]);
		std::vector<std::string> tokens = tokenize(" ", members.c_str());
		g_app->gui->getChatMembers()->clear();
		for (int x = 0; x<tokens.size(); x++)
		{
			const irr::core::stringw message3(tokens[x].c_str());
			g_app->gui->getChatMembers()->addItem((wchar_t*)message3.c_str());
		}
		break;
	}

	default:
		break;
	}

}


void Server::connectToServer()
{
	printf("Connecting to server: %s\n", config_.host.c_str());

    irc_ctx_t ctx;
	irc_callbacks_t callbacks;

	memset(&callbacks, 0, sizeof(irc_callbacks_t));

	callbacks.event_connect = event_connect;
	callbacks.event_join = event_join;
	callbacks.event_nick = dump_event;
	callbacks.event_quit = dump_event;
	callbacks.event_part = event_leave;
	callbacks.event_mode = dump_event;
	callbacks.event_topic = dump_event;
	callbacks.event_kick = dump_event;
	callbacks.event_channel = event_channel;
	callbacks.event_privmsg = event_privmsg;
	callbacks.event_notice = dump_event;
	callbacks.event_invite = dump_event;
	callbacks.event_umode = dump_event;
	callbacks.event_ctcp_rep = dump_event;
	callbacks.event_ctcp_action = dump_event;
	callbacks.event_unknown = dump_event;
	callbacks.event_numeric = event_numeric;

	callbacks.event_dcc_chat_req = irc_event_dcc_chat;
	callbacks.event_dcc_send_req = irc_event_dcc_send;

	irc_ = (void*)irc_create_session(&callbacks);
	if(!irc_) {
		printf("Couldn't create IRC session in Server.");
		return;
	}

	ctx.channel = const_cast<char*>(config_.channel.c_str());
    ctx.nick = const_cast<char*>(config_.nickName.c_str());

	irc_set_ctx (IRC, &ctx);

	std::string host = config_.host;
	if(config_.ssl) {
        irc_option_set( IRC, LIBIRC_OPTION_SSL_NO_VERIFY );
	}

	if(irc_connect(IRC, host.c_str(), config_.port, 0, config_.nickName.c_str(), 0, 0))
    {
        printf ("Could not connect: %s\n", irc_strerror (irc_errno(IRC)));
        return;
    }

    if ( irc_run (IRC) )
	{
		printf ("Could not connect or I/O error: %s\n", irc_strerror (irc_errno(IRC)));
		return;
	}


}

void Server::joinToChannel(std::string channel)
{
	irc_cmd_join (IRC, channel.c_str(), 0);
}

void Server::leaveToChannel(std::string channel)
{
	irc_cmd_part(IRC, channel.c_str());
}

void Server::message(const std::string channel, const std::string msg ) {

	irc_cmd_msg(IRC, channel.c_str(), msg.c_str());

}

void Server::me(const std::string channel, const std::string msg)
{
	irc_cmd_me(IRC, channel.c_str(), msg.c_str());
}

