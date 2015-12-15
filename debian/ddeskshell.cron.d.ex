#
# Regular cron jobs for the ddeskshell package
#
0 4	* * *	root	[ -x /usr/bin/ddeskshell_maintenance ] && /usr/bin/ddeskshell_maintenance
