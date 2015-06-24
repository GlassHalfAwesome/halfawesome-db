#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>

#define DAEMON_NAME "HalfAwesome Database"

void process() {
	// Do work
}   

int main(int argc, char *argv[]) {
	// Set our Logging Mask and open the Log
	setlogmask(LOG_UPTO(LOG_NOTICE));
	openlog(DAEMON_NAME, LOG_CONS | LOG_NDELAY | LOG_PERROR | LOG_PID, LOG_USER);

	syslog(LOG_INFO, "Entering Daemon");

	pid_t pid, sid;

	// Fork the Parent Process
	pid = fork();

	if (pid < 0) { exit(EXIT_FAILURE); }

	// We got a good pid, Close the Parent Process
	if (pid > 0) { exit(EXIT_SUCCESS); }

	// Change File Mask
	umask(0);

	// Create a new Signature Id for our child
	sid = setsid();
	if (sid < 0) { exit(EXIT_FAILURE); }

	// Change Directory
	// If we cant find the directory we exit with failure.
	if ((chdir("/hadb")) < 0) { exit(EXIT_FAILURE); }

	// Close Standard File Descriptors
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	// Main Process
	while(true){
		process();
		sleep(60);
	}

	// Close the log
	closelog ();

	return 0;
}
