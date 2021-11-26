const char *
container_type (void)
{
	struct stat  statbuf;
	char         buffer[1024];
	FILE        *f;
#if defined (PROCENV_PLATFORM_LINUX)
	dev_t        expected;

	expected = makedev (5, 1);
#endif

	if (stat ("/dev/console", &statbuf) < 0)
		goto out;

#if defined (PROCENV_PLATFORM_FREEBSD)
	if (misc.in_jail)
		return "jail";
#endif

	/* LXC's /dev/console is actually a pty */
#if defined (PROCENV_PLATFORM_LINUX)
	if (major (statbuf.st_rdev) != major (expected)
			|| (minor (statbuf.st_rdev)) != minor (expected))
		return "lxc";
#endif

	if (! stat ("/proc/vz", &statbuf) && stat ("/proc/bc", &statbuf) < 0)
		return "openvz";

	f = fopen ("/proc/self/status", "r");
	if (! f)
		goto out;

	while (fgets (buffer, sizeof (buffer), f)) {
		size_t len = strlen (buffer);
		buffer[len-1] = '\0';

		if (strstr (buffer, "VxID") == buffer) {
			fclose (f);
			return "vserver";
		}
	}

	fclose (f);

out:
	return "";
}




static int get_ncpu() {
	int ncpu;
#ifdef _SC_NPROCESSORS_ONLN
	ncpu = sysconf(_SC_NPROCESSORS_ONLN);
#elif defined __APPLE__
	int mib[2] = {CTL_HW, HW_NCPU};
	size_t len = sizeof(ncpu);
	sysctl(mib, 2, &ncpu, &len, NULL, 0);
#elif defined _GNU_SOURCE
	ncpu = get_nprocs();
#else
	ncpu = -1;
#endif
	return ncpu;
}

int get_pid_max()
{
#ifdef __linux__
	//read /proc/sys/kernel/pid_max
	static char buffer[1024];
	FILE *fd = fopen("/proc/sys/kernel/pid_max", "r");
	if (fd==NULL) return -1;
	if (fgets(buffer, sizeof(buffer), fd)==NULL) {
		fclose(fd);
		return -1;
	}
	fclose(fd);
	return atoi(buffer);
#elif defined __FreeBSD__
	return 99998;
#elif defined __APPLE__
	return 99998;
#endif
}


int
fd_valid (int fd)
{
	int flags = 0;

	if (fd < 0)
		return 0;

	errno = 0;
	flags = fcntl (fd, F_GETFL);

	if (flags < 0)
		return 0;

	/* redundant really */
	if (errno == EBADF)
		return 0;

	return 1;
}



static int tty_height(void)
{
#ifdef TIOCGWINSZ
	const int fd = 0;
	struct winsize ws;

	(void)memset(&ws, 0, sizeof(ws));
	/* if tty and we can get a sane width, return it */
	if (isatty(fd) &&
	    (ioctl(fd, TIOCGWINSZ, &ws) != -1) &&
	    (0 < ws.ws_row) &&
	    (ws.ws_row == (size_t)ws.ws_row))
		return ws.ws_row;
#endif
	return 25;	/* else standard tty 80x25 */
}
