#include <sys/types.h> // pid_t, umask()
#include <sys/stat.h> // umask()
#include <cstdlib> // exit()
#include <unistd.h> // setsid()
#include <syslog.h> // openlog(), syslog(), closelog(), setlogmask()

#include "process.h"

#define DAEMON_NAME "HalfAwesome Database"

int main(int argc, char *argv[]) {
	// Sets the log mask
	// I think this says to log only emerg/alert/crit/err/warning/notice and not info/debug
	setlogmask(LOG_UPTO(LOG_NOTICE));
	// Opens the log
	// I don't know what most of these parameters do
	openlog(DAEMON_NAME, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);

	// I don't know why we're using pid_t instead of an int, I guess for portability
	pid_t pid, sid;

	// Creates a child process for the daemon
	pid = fork();

	syslog(LOG_INFO, "Halfawesome Database Daemon started.");

	// Exits if it failed to create the child process
	// This is a bad thing
	if (pid < 0) {
		syslog(LOG_ERR, "Failed to create child process.");
		exit(EXIT_FAILURE);
	}

	// Exits the parent process if the child process was created successfully
	// This is a good thing
	if (pid > 0) {
		syslog(LOG_INFO, "Created child process.");
		exit(EXIT_SUCCESS);
	}

	// Change the file mask, 0 for full access to the generated files
	umask(0);

	// Creates a new session and sets the process group ID
	// Copied that from the man page. Still not 100% sure what that means
	sid = setsid();
	if (sid < 0) {
		syslog(LOG_ERR, "Failed to create a new session id.");
		exit(EXIT_FAILURE);
	}

	// Change to the db directory or exit if it fails
	if ((chdir("/hadb")) < 0) {
		syslog(LOG_ERR, "Failed to change to the db directory.");
		exit(EXIT_FAILURE);
	}

	// Close the standard file descriptors
	close(STDIN_FILENO);
	//close(STDOUT_FILENO);
	close(STDERR_FILENO);

	//int i = 0;
	// Main Process
	//while(i < 1){
		process(argv[1]);
		//sleep(1);
		//i++;
	//}

	// Close the log
	closelog ();

	return 0;
}
