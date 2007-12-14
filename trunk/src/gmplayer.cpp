#include "gmplayer.h"
#include <gdk/gdkx.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <string.h>
#include "ec_throw.h"
void  set_signals()
{
	sigset_t set;
	struct sigaction act;

	EC_THROW( -1 == sigfillset(&set));
	EC_THROW( -1 == sigprocmask(SIG_SETMASK, &set, NULL));

	memset(&act, 0, sizeof(act));

	EC_THROW( sigfillset(&act.sa_mask));
	act.sa_handler = SIG_IGN;
	EC_THROW( sigaction(SIGHUP, &act, NULL));
	EC_THROW( sigaction(SIGINT, &act, NULL));
	EC_THROW( sigaction(SIGQUIT, &act, NULL));
	EC_THROW( sigaction(SIGPIPE, &act, NULL));

	EC_THROW( sigaction(SIGTERM, &act, NULL));
	EC_THROW( sigaction(SIGBUS, &act, NULL));
	EC_THROW( sigaction(SIGFPE, &act, NULL));
	EC_THROW( sigaction(SIGILL, &act, NULL));
	EC_THROW( sigaction(SIGSEGV, &act, NULL));
	EC_THROW( sigaction(SIGSYS, &act, NULL));
	EC_THROW( sigaction(SIGXCPU, &act, NULL));
	EC_THROW( sigaction(SIGXFSZ, &act, NULL));
	EC_THROW( sigaction(SIGHUP, &act, NULL));

	EC_THROW( sigemptyset(&set));
	EC_THROW( sigprocmask(SIG_SETMASK, &set, NULL));
	
}

void GMplayer::set_s_pipe()
{
	close(stdin_pipe[1]);
	close(stdout_pipe[0]);
	EC_THROW( -1 == dup2(stdin_pipe[0], STDIN_FILENO));
	EC_THROW( -1 == dup2(stdout_pipe[1], STDOUT_FILENO));
	//EC_THROW( -1 == dup2(stdout_pipe[1], STDERR_FILENO));
}

GMplayer::GMplayer(const sigc::slot<bool, Glib::IOCondition>& slot):
	ready(true),
	childpid(-1),
	xid(-1),
	timer(0),
	child_call(slot)
	,mode(1)
{
	EC_THROW(-1 == pipe(stdin_pipe));
	EC_THROW(-1 == pipe(stdout_pipe));
}


GMplayer::~GMplayer()
{
	stop();
}

void GMplayer::wait_mplayer_exit(GPid pid, int)
{
	if (childpid != -1) {
		ptm_conn.disconnect();

		waitpid(childpid, NULL, 0);
		childpid = -1;

		ready = true;
		signal_stop_play_.emit();
	}
	
}

int GMplayer::my_system(char* const argv[])
{
	extern char **environ;
	ptm_conn = Glib::signal_io().connect(
			child_call,
			stdout_pipe[0], Glib::IO_IN);

	pid_t pid = fork();
	if (pid == -1)
		return -1;
	if (pid == 0) {

		set_signals();
		set_s_pipe();
		//sleep(4);
		execvp(argv[0], argv);

		perror("mplayer execvp:");
		exit(127);
	} 
	childpid = pid;
	wait_conn = Glib::signal_child_watch().connect(
			sigc::mem_fun(*this, &GMplayer::wait_mplayer_exit), pid, 0);

	return pid;


}

bool GMplayer::is_runing()
{
	return (!ready) && (childpid > 0);
}

void GMplayer::start()
{
	if (is_runing()) 
		stop();

	if (file.empty())
		return;

	signal_start_play_.emit();

	Glib::RefPtr<Gdk::Window> gwin = this->get_window();
	int x,y,width,heigh,depth;
	if(gwin)
	{
		xid=GDK_WINDOW_XID(gwin->gobj());
		gwin->get_geometry(x,y,width,heigh,depth);
	}

	char wid_buf[32];
	snprintf(wid_buf, 32, "%d", xid);

	const char* argv[6];
	argv[0] = "mplayer";
	argv[1] = "-slave";
	argv[2] = "-wid";
	argv[3] = wid_buf;
	argv[4] = file.c_str();
	argv[5] = NULL;

	ready = false;
	my_system((char* const *) argv);
}

void GMplayer::start(const std::string& filename)
{
	if (filename != file)
		file = filename;
	start();
}

void GMplayer::full_screen()
{
}

void GMplayer::send_ctrl_word(char c)
{
	char cb[3];
	sprintf(cb, "%c\n", c);
	EC_THROW(-1 == write(stdin_pipe[1], cb, sizeof (cb)));
}

void GMplayer::stop()
{
	if (childpid != -1) {
		ptm_conn.disconnect();
		wait_conn.disconnect();
		kill(childpid, SIGKILL);
		waitpid(childpid, NULL, 0);
		childpid = -1;	
		signal_stop_play_.emit();
	}
	ready = true;
}

