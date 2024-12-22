/* common.h
 */
/* This software is copyrighted as detailed in the LICENSE file. */


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <ctype.h>
#include "config.h"	/* generated by installation script */
#include "config2.h"

#include <errno.h>
#include <signal.h>
#ifdef I_SYS_FILIO
# include <sys/filio.h>
#endif
#ifdef I_SYS_IOCTL
# include <sys/ioctl.h>
#endif
#ifdef I_VFORK
# include <vfork.h>
#endif
#include <fcntl.h>

#ifdef I_TERMIO
# include <termio.h>
#else
# ifdef I_TERMIOS
#   include <termios.h>
#   if !defined (O_NDELAY)
#     define O_NDELAY O_NONBLOCK	/* Posix-style non-blocking i/o */
#   endif
# else
#   ifdef I_SGTTY
#     include <sgtty.h>
#   endif
# endif
#endif

#ifdef I_PTEM
#include <sys/stream.h>
#include <sys/ptem.h>
#endif

#ifdef I_TIME
#include <time.h>
#endif
#ifdef I_SYS_TIME
#include <sys/time.h>
#endif

#include "typedef.h"

#define BITSPERBYTE 8
#define LBUFLEN 2048	/* line buffer length */
			/* (don't worry, .newsrc lines can exceed this) */
#define CBUFLEN 512	/* command buffer length */
#define PUSHSIZE 256
#define MAXFILENAME 512
#define FINISHCMD 0177
#define OV_MAX_FIELDS 9

/* Things we can figure out ourselves */

#ifdef SIGTSTP
#   define BERKELEY 	/* include job control signals? */
#endif

#if defined(FIONREAD) || defined(HAVE_RDCHK) || defined(O_NDELAY) || defined(MSDOS)
#   define PENDING
#endif

#ifdef EUNICE
#   define LINKART		/* add 1 level of possible indirection */
#   define UNLINK(victim) while (!unlink(victim))
#else
#   define UNLINK(victim) unlink(victim)
#endif

#ifdef HAVE_RENAME
#   define RENAME(from,to) rename(from,to)
#else
#   define RENAME(from,to) safelink(from,to), UNLINK(from)
#endif

#ifdef HAVE_STRSTR
#   define STRSTR(s1,s2) strstr((s1),(s2))
#else
#   define STRSTR(s1,s2) trn_strstr((s1),(s2))
#endif

/* Valid substitutions for strings marked with % comment are:
 *	%a	Current article number
 *	%A	Full name of current article (%P/%c/%a)
 *		(if LINKART defined, is the name of the real article)
 *	%b	Destination of a save command, a mailbox or command
 *	%B	The byte offset to the beginning of the article for saves
 *		with or without the header
 *	%c	Current newsgroup, directory form
 *	%C	Current newsgroup, dot form
 *	%d	%P/%c
 *	%D	Old Distribution: line
 *	%e	Extract program
 *	%E	Extract destination directory
 *	%f	Old From: line or Reply-To: line
 *	%F	Newsgroups to followup to from Newsgroups: and Followup-To:
 *	%h	Name of header file to pass to mail or news poster
 *	%H	Host name (yours)
 *	%i	Old Message-I.D.: line, with <>
 *	%j	The terminal speed (e.g. 9600)
 *	%I	Inclusion indicator
 *	%l	News administrator login name
 *	%L	Login name (yours)
 *	%m	The current mode of trn.
 *	%M	Number of articles marked with M
 *	%n	Newsgroups from source article
 *	%N	Full name (yours)
 *	%o	Organization (yours)
 *	%O	Original working directory (where you ran trn from)
 *	%p	Your private news directory (-d switch)
 *	%P	Public news spool directory (NEWSSPOOL)
 *	%r	Last reference (parent article id)
 *	%q	The last quoted input (via %").
 *	%R	New references list
 *	%s	Subject, with all Re's and (nf)'s stripped off
 *	%S	Subject, with one Re stripped off
 *	%t	New To: line derived from From: and Reply-To (Internet always)
 *	%T	New To: line derived from Path:
 *	%u	Number of unread articles
 *	%U	Number of unread articles disregarding current article
 *	%v	Number of unselected articles disregarding current article
 *	%W	The thread directory root
 *	%x	News library directory, usually /usr/lib/news
 *	%X	Trn's private library directory, usually %x/trn
 *	%y	From line with domain shortening (name@*.domain.nam)
 *	%Y	The tmp directory to use
 *	%z	Size of current article in bytes.
 *	%Z	Number of selected threads.
 *	%~	Home directory
 *	%.	Directory containing . files, usually %~
 *	%+	Directory containing a user's init files, usually %./.trn
 *	%#	count of articles saved in current command (from 1 to n)
 *	%^#	ever-increasing number (from 1 to n)
 *	%$	current process number
 *	%{name} Environment variable "name".  %{name-default} form allowed.
 *	%[name]	Header line beginning with "Name: ", without "Name: " 
 *	%"prompt"
 *		Print prompt and insert what is typed.
 *	%`command`
 *		Insert output of command.
 *	%(test_text=pattern?if_text:else_text)
 *		Substitute if_text if test_text matches pattern, otherwise
 *		substitute else_text.  Use != for negated match.
 *		% substitutions are done on test_text, if_text, and else_text.
 *	%digit	Substitute the text matched by the nth bracket in the last
 *		pattern that had brackets.  %0 matches the last bracket
 *		matched, in case you had alternatives.
 *	%?	Insert a space unless the entire result is > 79 chars, in
 *		which case the space becomes a newline.
 *
 *	Put ^ in the middle to capitalize the first letter: %^C = Rec.humor
 *	Put _ in the middle to capitalize last component: %_c = net/Jokes
 *	Put \ in the middle to quote regexp and % characters in the result
 *	Put > in the middle to return the address portion of a name.
 *	Put ) in the middle to return the comment portion of a name.
 *	Put ' in the middle to protect "'"s in arguments you've put in "'"s.
 *	Put :FMT in the middle to format the result: %:-30.30t
 *
 *	~ interpretation in filename expansion happens after % expansion, so
 *	you could put ~%{NEWSLOGNAME-news} and it will expand correctly.
 */

/* *** System Dependent Stuff *** */

/* NOTE: many of these are defined in the config.h file */

/* name of organization */
#ifndef ORGNAME
#   define ORGNAME "ACME Widget Company, Widget Falls, Southern North Dakota"
#endif

#ifndef MBOXCHAR
#   define MBOXCHAR 'F'	/* how to recognize a mailbox by 1st char */
#endif

#ifndef ROOTID
#   define ROOTID 0        /* uid of superuser */
#endif

#ifdef NORMSIG
#   define sigset signal
#   define sigignore(sig) signal(sig,SIG_IGN)
#endif

#ifndef PASSFILE
#   ifdef LIMITED_FILENAMES
#	define PASSFILE "%X/passwd"
#   else
#	define PASSFILE "/etc/passwd"
#   endif
#endif

#ifndef LOGDIRFIELD
#   define LOGDIRFIELD 6		/* Which field (origin 1) is the */
					/* login directory in /etc/passwd? */
					/* (If it is not kept in passwd, */
					/* but getpwnam() returns it, */
					/* define the symbol HAVE_GETPWENT) */
#endif
#ifndef GCOSFIELD
#   define GCOSFIELD 5
#endif

#ifndef NEGCHAR
#   define NEGCHAR '!'
#endif

/* Space conservation section */

/* To save D space, cut down size of MAXNGTODO and VARYSIZE. */
#define MAXNGTODO 512	/* number of newsgroups allowed on command line */
#define VARYSIZE 256	/* this makes a block 1024 bytes long in DECville */
			/* (used by virtual array routines) */

/* Undefine any of the following features to save both I and D space */
/* In general, earlier ones are easier to get along without */
#define USE_FILTER	/* external kill/score filter */
#define CUSTOMLINES	/* include code for HIDELINE and PAGESTOP */
#define WORDERASE	/* enable ^W to erase a word */
#define MAILCALL	/* check periodically for mail */
#define NOFIREWORKS	/* keep whole screen from flashing on certain */
			/* terminals such as older Televideos */
#define VERIFY		/* echo the command they just typed */
#define BACKTICK	/* allow %`command` */
#define PROMPTTTY	/* allow %"prompt" */
#define ULSMARTS	/* catch _^H in text and do underlining */
#define TERMMOD		/* allow terminal type modifier on switches */
#define BAUDMOD		/* allow baudrate modifier on switches */
#define ORGFILE		/* if organization begins with /, look up in file */
#define TILDENAME	/* allow ~logname expansion */
#define SETENV		/* allow command line environment variable setting */
#define MAKEDIR		/* use our makedir() instead of shell script */
#define MEMHELP		/* keep help messages in memory */
#define VERBOSE		/* compile in more informative messages */
#define TERSE		/* compile in shorter messages */
			/* (Note: both VERBOSE and TERSE can be defined; -t
			 * sets terse mode.  One or the other MUST be defined.
			 */
#define CHARSUBST	/* enable the _C command */
#define DELBOGUS	/* ask if bogus newsgroups should be deleted */
#define RELOCATE	/* allow newsgroup rearranging */
#define ESCSUBS		/* escape substitutions in multi-character commands */
#undef MCHASE		/* unmark xrefed articles on m or M */
#define MUNGHEADER	/* allow alternate header formatting via */
			/* environment variable ALTHEADER (not impl) */
#define ASYNC_PARSE	/* allow parsing headers asyncronously to reading */
			/* used by MCHASE and MUNGHEADER */
#define FINDNEWNG	/* check for new newsgroups on startup */
#define FASTNEW		/* do optimizations on FINDNEWNG for faster startup */
#define INNERSEARCH	/* search command 'g' with article */
#define CATCHUP		/* catchup command at newsgroup level */
#define NGSEARCH	/* newsgroup pattern matching */
#define KILLFILES	/* automatic article killer files */
#define ARTSEARCH	/* pattern searches among articles */
			/* /, ?, ^N, ^P, k, K */
#define EDIT_DISTANCE	/* Allow -G to specify a fuzzy 'go' command */
#undef	VALIDATE_XREF_SITE /* are xrefs possibly invalid? */

/*#define LONG_THREAD_NAMES	*//**/

/* USEURL may soon become a Configure script option */
/*#define USEURL  *//* allow trn to fetch/use WWW URLs */

/* if USEFTP is defined, trn will use the ftpgrab script for ftp: URLs
 * USEFTP is not very well tested, and the ftpgrab script is not
 * installed with make install.  May go away later
 */
/*#define USEFTP  *//**/

/* The NICEBG functions are probably only useful now for *slow* systems */
/*#define NICEBG  *//* use some kind of nice-background call */
/* the specific type of NICEBG is used only if NICBG is defined */
/* select() call for nice background. */
#define NBG_SELECT   /**/
/* TERMIO(S) style of nice background available.  Second choice */
/*#define NBG_TERMIO *//**/
/* SIGIO style of nice background (last resort) */
/*#define NBG_SIGIO  *//**/

/* SHORTSCORENAMES has not been tested recently */
/*#define SHORTSCORENAMES  *//* foo/bar/baz/SCORE instead of foo.bar.baz */

/* if SCOREFILE_CACHE is defined, scorefiles will be kept in memory. */
#define SCOREFILE_CACHE

/* some dependencies among options */

#ifdef SCAN
# define SCAN_ART
#else
# ifdef SCAN_ART
#  undef SCAN_ART
# endif
#endif /* SCAN */

#ifndef SCORE
# ifdef USE_FILTER
#  undef USE_FILTER
# endif
#endif /* SCORE */

#ifndef ARTSEARCH
#   undef KILLFILES
#   undef INNERSEARCH
#endif

#ifndef SETUIDGID
#   define eaccess access
#endif

#ifdef VERBOSE
#   ifdef TERSE
#	define IF(c) if (c)
#	define ELSE else
#   else
#	define IF(c)
#	define ELSE
#   endif
#else /* !VERBOSE */
#   ifndef TERSE
#	define TERSE
#   endif
#   define IF(c) ..."IF" outside of VERBOSE
#   define ELSE ..."ELSE" outside of VERBOSE
#endif

#ifdef SUPPORT_NNTP
#define ElseIf else if
#else
#define ElseIf if
#endif

#ifdef DEBUG
#   define assert(ex) {if (!(ex)){fprintf(stderr,"Assertion failed: file %s, line %d\n", __FILE__, __LINE__);sig_catcher(0);}}
#else
#   define assert(ex) ;
#endif

/* If you're strapped for space use the help messages in shell scripts */
/* if {NG,ART,PAGER,SUBS}HELP is undefined, help messages are in memory */
#ifdef MEMHELP  /* undef MEMHELP above to get them all as sh scripts */
#   undef NGHELP
#   undef ARTHELP
#   undef PAGERHELP
#   undef SUBSHELP
#else
#   ifndef NGHELP			/* % and ~ */
#	define NGHELP "%X/ng.help"
#   endif
#   ifndef ARTHELP			/* % and ~ */
#	define ARTHELP "%X/art.help"
#   endif
#   ifndef PAGERHELP		/* % and ~ */
#	define PAGERHELP "%X/pager.help"
#   endif
#   ifndef SUBSHELP		/* % and ~ */
#	define SUBSHELP "%X/subs.help"
#   endif
#endif

#define TCSIZE 512	/* capacity for termcap strings */

#ifdef EDIT_DISTANCE
#   define MIN_DIST 7	/* Maximum error count for acceptable match */
#endif

/* Additional ideas:
 *	Make the do_newsgroup() routine a separate process.
 *	Keep .newsrc on disk instead of in memory.
 *	Overlays, if you have them.
 *	Get a bigger machine.
 */

/* End of Space Conservation Section */

/* More System Dependencies */

/* news library */
#ifndef NEWSLIB		/* ~ and %l only ("~%l" is permissable) */
#   define NEWSLIB "/usr/lib/news"
#endif

/* path to private executables */
#ifndef PRIVLIB		/* ~, %x and %l only */
#   define PRIVLIB "%x/trn"
#endif

/* system-wide RNINIT switches */
#ifndef GLOBINIT
#   define GLOBINIT "%X/INIT"
#endif

/* where to find news files */
#ifndef NEWSSPOOL		/* % and ~ */
#   define NEWSSPOOL "/usr/spool/news"
#endif

#ifndef THREAD_DIR
#   undef LONG_THREAD_NAMES
#endif

/* default characters to use in the selection menu */
#ifndef SELECTCHARS
#   define SELECTCHARS "abdefgijlorstuvwxyz1234567890BCFGIKVW"
#endif

/* file containing list of active newsgroups and max article numbers */
#ifndef ACTIVE			/* % and ~ */
#   define ACTIVE "%x/active"
#endif
#ifndef ACTIVE_TIMES
#   define ACTIVE_TIMES "none"
#endif
#ifndef GROUPDESC
#   define GROUPDESC "%x/newsgroups"
#endif
#ifndef DBINIT
#   define DBINIT "%W/db.init"
#endif

/* location of history file */
#ifndef ARTFILE			/* % and ~ */
#    define ARTFILE "%x/history"
#endif

/* preferred shell for use in doshell routine */
/*  ksh or sh would be okay here */
#ifndef PREFSHELL
#   define PREFSHELL "/bin/csh"
#endif

/* path to fastest starting shell */
#ifndef SH
#   define SH "/bin/sh"
#endif

/* path to default editor */
#ifndef DEFEDITOR
#   define DEFEDITOR "/usr/ucb/vi"
#endif

/* the user's init files */
#ifndef TRNDIR
# ifdef LIMITED_FILENAMES
#   define TRNDIR "%./trn"
# else
#   define TRNDIR "%./.trn"
# endif
#endif

/* location of macro file for trn and rn modes */
#ifndef TRNMACRO
#   define TRNMACRO "%+/macros"
#endif
#ifndef RNMACRO
# ifdef LIMITED_FILENAMES
#   define RNMACRO "%./rnmac"
# else
#   define RNMACRO "%./.rnmac"
# endif
#endif

/* location of full name */
#ifndef FULLNAMEFILE
#   ifndef PASSNAMES
#     ifdef LIMITED_FILENAMES
#	define FULLNAMEFILE "%./fullname"
#     else
#	define FULLNAMEFILE "%./.fullname"
#     endif
#   endif
#endif

/* The name to append to the directory name to read an overview file. */
#ifndef OV_FILE_NAME
# ifdef LIMITED_FILENAMES
#   define OV_FILE_NAME "/overview"
# else
#   define OV_FILE_NAME	"/.overview"
# endif
#endif

/* The name to append to the directory name to read a thread file. */
#ifndef MT_FILE_NAME
# ifdef LIMITED_FILENAMES
#   define MT_FILE_NAME "/thread"
# else
#   define MT_FILE_NAME	"/.thread"
# endif
#endif

/* virtual array file name template */
#ifndef VARYNAME		/* % and ~ */
#   define VARYNAME "%Y/rnvary.%$"
#endif

/* file to pass header to followup article poster */
#ifndef HEADNAME		/* % and ~ */
#   ifdef LIMITED_FILENAMES
#	define HEADNAME "%Y/tmpart.%$"
#   else
#	define HEADNAME "%./.rnhead.%$"
#   endif
#endif

#ifndef MAKEDIR
/* shell script to make n-deep subdirectories */
#   ifndef DIRMAKER		/* % and ~ */
#	define DIRMAKER "%X/makedir"
#   endif
#endif

/* trn's default access list */
#ifndef DEFACCESS
#   define DEFACCESS "%X/access.def"
#endif

/* trn's access list */
#ifndef TRNACCESS
#   define TRNACCESS "%+/access"
#endif

/* location of newsrc file */
#ifndef RCNAME		/* % and ~ */
# ifdef LIMITED_FILENAMES
#   define RCNAME "%./newsrc"
# else
#   define RCNAME "%./.newsrc"
# endif
#endif

/* temporary newsrc file in case we crash while writing out */
#ifndef RCNAME_NEW
#   define RCNAME_NEW "%s.new"
#endif

/* newsrc file at the beginning of this session */
#ifndef RCNAME_OLD
#   define RCNAME_OLD "%s.old"
#endif

/* lockfile for each newsrc that is not ~/.newsrc (which uses .rnlock) */
#ifndef RCNAME_LOCK
#   define RCNAME_LOCK "%s.LOCK"
#endif

/* news source info for each newsrc that is not ~/.newsrc (.rnlast) */
#ifndef RCNAME_INFO
#   define RCNAME_INFO "%s.info"
#endif

/* if existent, contains process number of current or crashed trn */
#ifndef LOCKNAME		/* % and ~ */
# ifdef LIMITED_FILENAMES
#   define LOCKNAME "%+/lock"
# else
#   define LOCKNAME "%./.rnlock"
# endif
#endif

/* information from last invocation of trn */
#ifndef LASTNAME		/* % and ~ */
# ifdef LIMITED_FILENAMES
#   define LASTNAME "%+/rnlast"
# else
#   define LASTNAME "%./.rnlast"
# endif
#endif

#ifndef SIGNATURE_FILE
# ifdef LIMITED_FILENAMES
#   define SIGNATURE_FILE "%./signatur"
# else
#   define SIGNATURE_FILE "%./.signature"
# endif
#endif

#ifndef NNTP_AUTH_FILE
# define NNTP_AUTH_FILE "%./.nntpauth"
#endif

/* a motd-like file for trn */
#ifndef NEWSNEWSNAME		/* % and ~ */
#   define NEWSNEWSNAME "%X/newsnews"
#endif

/* command to send a reply */
#ifndef MAILPOSTER		/* % and ~ */
#   define MAILPOSTER "Rnmail -h %h"
#endif

#ifdef INTERNET
#   ifndef MAILHEADER		/* % */
#	define MAILHEADER "To: %t\nSubject: %(%i=^$?:Re: %S\nX-Newsgroups: %n\nIn-Reply-To: %i)\n%(%{FROM}=^$?:From: %{FROM}\n)%(%{REPLYTO}=^$?:Reply-To: %{REPLYTO}\n)%(%[references]=^$?:References: %[references]\n)Organization: %o\nCc: \nBcc: \n\n"
#   endif
#else
#   ifndef MAILHEADER		/* % */
#	define MAILHEADER "To: %T\nSubject: %(%i=^$?:Re: %S\n%(%{FROM}=^$?:From: %{FROM}\n)%(%{REPLYTO}=^$?:Reply-To: %{REPLYTO}\n)X-Newsgroups: %n\nIn-Reply-To: %i)\n%(%[references]=^$?:References: %[references]\n)Organization: %o\nCc: \nBcc: \n\n"
#   endif
#endif

#ifndef YOUSAID			/* % */
#   define YOUSAID "In article %i you write:"
#endif

/* command to forward an article */
#define FORWARDPOSTER MAILPOSTER

#ifdef INTERNET
#   ifndef FORWARDHEADER	/* % */
#	define FORWARDHEADER "To: %\"\n\nTo: \"\nSubject: %(%i=^$?:%[subject] (fwd\\)\n%(%{FROM}=^$?:From: %{FROM}\n)%(%{REPLYTO}=^$?:Reply-To: %{REPLYTO}\n)X-Newsgroups: %n\nIn-Reply-To: %i)\n%(%[references]=^$?:References: %[references]\n)Organization: %o\nMime-Version: 1.0\nContent-Type: multipart/mixed; boundary=\"=%$%^#=--\"\nCc: \nBcc: \n\n"
#   endif
#else
#   ifndef FORWARDHEADER	/* % */
#	define FORWARDHEADER "To: %\"\n\nTo: \"\nSubject: %(%i=^$?:%[subject] (fwd\\)\n%(%{FROM}=^$?:From: %{FROM}\n)%(%{REPLYTO}=^$?:Reply-To: %{REPLYTO}\n)X-Newsgroups: %n\nIn-Reply-To: %i)\n%(%[references]=^$?:References: %[references]\n)Organization: %o\nMime-Version: 1.0\nContent-Type: multipart/mixed; boundary=\"=%$%^#=--\"\nCc: \nBcc: \n\n"
#   endif
#endif

#ifndef FORWARDMSG		/* % */
#   define FORWARDMSG "------- start of forwarded message -------"
#endif

#ifndef FORWARDMSGEND		/* % */
#   define FORWARDMSGEND "------- end of forwarded message -------"
#endif

/* command to submit a followup article */
#ifndef NEWSPOSTER		/* % and ~ */
#   define NEWSPOSTER "Pnews -h %h"
#endif

#ifndef NEWSHEADER		/* % */
#   define NEWSHEADER "%(%[followup-to]=^$?:%(%[followup-to]=^%n$?:X-ORIGINAL-NEWSGROUPS: %n\n))Newsgroups: %(%F=^$?%C:%F)\nSubject: %(%S=^$?%\"\n\nSubject: \":Re: %S)\nSummary: \nExpires: \n%(%R=^$?:References: %R\n)Sender: \nFollowup-To: \n%(%{FROM}=^$?:From: %{FROM}\n)%(%{REPLYTO}=^$?:Reply-To: %{REPLYTO}\n)Distribution: %(%i=^$?%\"Distribution: \":%D)\nOrganization: %o\nKeywords: %[keywords]\nCc: %(%F=poster?%t:%(%F!=@?:%F))\n\n"
#endif

#ifndef ATTRIBUTION		/* % */
#   define ATTRIBUTION "In article %i,%?%)f <%>f> wrote:"
#endif

#ifndef PIPESAVER		/* % */
#   define PIPESAVER "%(%B=^0$?<%A:tail -c +%B %A |) %b"
#endif

#ifndef SHARSAVER
#   define SHARSAVER "tail -c +%B %A | /bin/sh"
#endif

#ifndef CUSTOMSAVER
#   define CUSTOMSAVER "tail -c +%B %A | %e"
#endif

#ifndef VERIFY_RIPEM
#   define VERIFY_RIPEM "ripem -d -Y fgs -i %A"
#endif

#ifndef VERIFY_PGP
#   define VERIFY_PGP "pgp +batchmode -m %A"
#endif

#ifdef MKDIRS

#   ifndef SAVEDIR			/* % and ~ */
#	define SAVEDIR "%p/%c"
#   endif
#   ifndef SAVENAME		/* % */
#	define SAVENAME "%a"
#   endif

#else

#   ifndef SAVEDIR			/* % and ~ */
#	define SAVEDIR "%p"
#   endif
#   ifndef SAVENAME		/* % */
#	define SAVENAME "%^C"
#   endif

#endif

#ifndef KILLGLOBAL		/* % and ~ */
#   define KILLGLOBAL "%p/KILL"
#endif

#ifndef KILLLOCAL		/* % and ~ */
#   define KILLLOCAL "%p/%c/KILL"
#endif

#ifndef KILLTHREADS		/* % and ~ */
#   define KILLTHREADS "%+/Kill/Threads"
#endif

#ifdef USE_FILTER
#  ifndef FILTERPROG
#    define FILTERPROG "%+/filter"
#  endif
#endif /* USE_FILTER */

/* how to cancel an article */
#ifndef CALL_INEWS
#   ifdef BNEWS
#	define CALL_INEWS "%x/inews -h <%h"
#   else
#	define CALL_INEWS "inews -h <%h"
#   endif
#endif

/* how to cancel an article, continued */
#ifndef CANCELHEADER
#   define CANCELHEADER "Newsgroups: %n\nSubject: cancel\n%(%{FROM}=^$?:From: %{FROM}\n)Control: cancel %i\nDistribution: %D\n\n%i was cancelled from within trn.\n"
#endif

/* how to supersede an article */
#ifndef SUPERSEDEHEADER
#   define SUPERSEDEHEADER "Newsgroups: %n\nSubject: %[subject]\n%(%{FROM}=^$?:From: %{FROM}\n)Summary: %[summary]\nExpires: %[expires]\nReferences: %[references]\nFrom: %[from]\nReply-To: %[reply-to]\nSupersedes: %i\nSender: %[sender]\nFollowup-To: %[followup-to]\nDistribution: %D\nOrganization: %o\nKeywords: %[keywords]\n\n"
#endif

#ifndef LOCALTIMEFMT
#   define LOCALTIMEFMT "%a %b %d %X %Z %Y"
#endif

/* where to find the mail file */
#ifndef MAILFILE
#   define MAILFILE "/usr/spool/mail/%L"
#endif

/* dependencies of nice background scoring */
#ifdef PENDING
# ifdef NICEBG
#  ifndef I_TERMIO
#   ifndef I_TERMIOS
#    undef NBG_TERMIO
#   endif
#  endif
/* later insert defines to turn off NBG_SIGIO when needed */
# endif
#else /* !PENDING */
#   undef NICEBG
#endif

#ifndef HAVE_VFORK
#   define vfork fork
#endif

/* Winsock is only initialized if NNTP_SUPPORT is defined */
#ifdef WINSOCK
# ifndef NNTP_SUPPORT
#  ifdef USEURL
#   undef USEURL
#  endif
# endif
#endif

/* TK requires TCL */
#ifdef USE_TK
# ifndef USE_TCL
#  define USE_TCL
# endif
#endif

/* *** end of the machine dependent stuff *** */

/* GLOBAL THINGS */

/* file statistics area */

EXT struct stat filestat;

/* various things of type char */

EXT char msg[CBUFLEN];		/* general purpose message buffer */
EXT char buf[LBUFLEN+1];	/* general purpose line buffer */
EXT char cmd_buf[CBUFLEN];	/* buffer for formatting system commands */
EXT char* indstr INIT(">");	/* indent for old article embedded in followup */

EXT char* cwd INIT(NULL);	/* current working directory */
EXT char* dfltcmd INIT(NULL);	/* 1st char is default command */

/* switches */

#ifdef DEBUG
    EXT int debug INIT(0);				/* -D */
#   define DEB_COREDUMPSOK 2
#   define DEB_HEADER 4
#   define DEB_INTRP 8
#   define DEB_NNTP 16
#   define DEB_INNERSRCH 32
#   define DEB_FILEXP 64 
#   define DEB_HASH 128
#   define DEB_XREF_MARKER 256
#   define DEB_CTLAREA_BITMAP 512
#   define DEB_RCFILES 1024
#   define DEB_NEWSRC_LINE 2048
#   define DEB_SEARCH_AHEAD 4096
#   define DEB_CHECKPOINTING 8192
#   define DEB_FEED_XREF 16384
#endif

#ifdef ARTSEARCH
EXT int scanon INIT(0);					/* -S */
#endif

EXT bool use_threads INIT(THREAD_INIT);			/* -x */
EXT int max_tree_lines INIT(6);

EXT char UnivSelCmds[3] INIT("Z>");
EXT char NewsrcSelCmds[3] INIT("Z>");
EXT char AddSelCmds[3] INIT("Z>");
EXT char NewsgroupSelCmds[3] INIT("Z>");
EXT char NewsSelCmds[3] INIT("Z>");
EXT char OptionSelCmds[3] INIT("Z>");

EXT int UnivSelBtnCnt;
EXT int NewsrcSelBtnCnt;
EXT int AddSelBtnCnt;
EXT int NewsgroupSelBtnCnt;
EXT int NewsSelBtnCnt;
EXT int OptionSelBtnCnt;
EXT int ArtPagerBtnCnt;

EXT char* UnivSelBtns INIT(NULL);
EXT char* NewsrcSelBtns INIT(NULL);
EXT char* AddSelBtns INIT(NULL);
EXT char* NewsgroupSelBtns INIT(NULL);
EXT char* NewsSelBtns INIT(NULL);
EXT char* OptionSelBtns INIT(NULL);
EXT char* ArtPagerBtns INIT(NULL);

EXT bool dont_filter_control INIT(FALSE);		/* -j */
EXT int  join_subject_len INIT(0);			/* -J */
EXT bool kill_thru_kludge INIT(TRUE);			/* -k */
EXT bool keep_the_group_static INIT(FALSE);		/* -K */
EXT bool mbox_always INIT(FALSE);			/* -M */
EXT bool norm_always INIT(FALSE);			/* -N */
EXT bool thread_always INIT(FALSE);			/* -a */
EXT int auto_arrow_macros INIT(2);			/* -A */
EXT bool breadth_first INIT(FALSE);			/* -b */
EXT bool bkgnd_spinner INIT(FALSE);			/* -B */
EXT bool novice_delays INIT(TRUE);			/* +f */
EXT int olden_days INIT(FALSE);				/* -o */
EXT char auto_select_postings INIT(0);			/* -p */
EXT bool checkflag INIT(FALSE);				/* -c */
EXT char* savedir INIT(NULL);				/* -d */
EXT bool suppress_cn INIT(FALSE);			/* -s */
EXT int countdown INIT(5);	/* how many lines to list before invoking -s */
EXT bool muck_up_clear INIT(FALSE);			/* -loco */
EXT bool erase_screen INIT(FALSE);			/* -e */
EXT bool can_home INIT(FALSE);
EXT bool erase_each_line INIT(FALSE);			/* fancy -e */
EXT int findlast INIT(0);				/* -r */
EXT bool allow_typeahead INIT(FALSE);			/* -T */
#ifdef EDIT_DISTANCE
EXT bool fuzzyGet INIT(FALSE);				/* -G */
#endif
#ifdef VERBOSE
#   ifdef TERSE
EXT bool verbose INIT(TRUE);				/* +t */
#   endif
#endif
EXT bool unbroken_subjects INIT(FALSE);			/* -u */
EXT bool unsafe_rc_saves INIT(FALSE);			/* -U */
#ifdef VERIFY
EXT bool verify INIT(FALSE);				/* -v */
#endif
EXT bool quickstart INIT(FALSE);			/* -q */

#define DEFAULT_REFETCH_SECS  4L*60*60	/* 4 hours */
EXT time_t defRefetchSecs INIT(DEFAULT_REFETCH_SECS);	/* -z */

EXT int word_wrap_offset INIT(8);  /* right-hand column size (0 is off) */

#define NOMARKING 0
#define STANDOUT 1
#define UNDERLINE 2
#define LASTMARKING 3
EXT int marking INIT(NOMARKING);			/* -m */
#define HALFPAGE_MARKING 1
#define BACKPAGE_MARKING 2
EXT int marking_areas INIT(HALFPAGE_MARKING);

EXT ART_LINE initlines INIT(0);				/* -i */
EXT bool append_unsub					/* -I */
#ifdef APPEND_UNSUB
     INIT(1);
#else
     INIT(0);
#endif

EXT bool UseUnivSelector INIT(FALSE);
EXT bool UseNewsrcSelector INIT(FALSE);
EXT bool UseAddSelector INIT(TRUE);
EXT bool UseNewsgroupSelector INIT(TRUE);
EXT int UseNewsSelector INIT(SELECT_INIT-1);
EXT bool UseMouse INIT(FALSE);
EXT char MouseModes[32] INIT("acjlptwvK");
EXT bool use_colors INIT(FALSE);
EXT bool UseTk INIT(FALSE);
EXT bool UseTcl INIT(FALSE);
EXT bool UseSelNum INIT(FALSE);
EXT bool SelNumGoto INIT(FALSE);
/* miscellania */

EXT bool in_ng INIT(FALSE);		/* TRUE if in a newsgroup */
EXT char mode INIT('i');		/* current state of trn */
EXT char gmode INIT('I');		/* general mode of trn */

EXT FILE* tmpfp INIT(NULL);	/* scratch fp used for .rnlock, .rnlast, etc. */

#define PLURAL(num) ((num)==1? nullstr : ess)
#define ALLBITS(val,bits) (((val) & (bits)) == (bits))

/* Factored strings */

EXT char nullstr[1] INIT("");
EXT char ess[2] INIT("s");
EXT char sh[] INIT(SH);
EXT char defeditor[] INIT(DEFEDITOR);
EXT char hforhelp[] INIT("Type h for help.\n");
#ifdef STRICTCR
EXT char badcr[] INIT("\nUnnecessary CR ignored.\n");
#endif
EXT char readerr[] INIT("rn read error");
EXT char unsubto[] INIT("Unsubscribed to newsgroup %s\n");
EXT char cantopen[] INIT("Can't open %s\n");
EXT char cantcreate[] INIT("Can't create %s\n");
EXT char cantrecreate[] INIT("Can't recreate %s -- restoring older version.\n\
Perhaps you are near or over quota?\n");

#ifdef VERBOSE
    EXT char nocd[] INIT("Can't chdir to directory %s\n");
#else
    EXT char nocd[] INIT("Can't find %s\n");
#endif

#ifdef NOLINEBUF
#define FLUSH ,fflush(stdout)
#else
#define FLUSH
#endif

#ifdef lint
#undef FLUSH
#define FLUSH
#undef putchar
#define putchar(c)
#endif

#define nntp_advise(str) fputs(str,stdout)
#define nntp_init_error(str) fputs(str,stdout)
#define nntp_error(str) fputs(str,stderr)
#define NNTP_ERROR_IS_FATAL
#define NNTP_HANDLE_TIMEOUT
#define NNTP_HANDLE_AUTH_ERR
