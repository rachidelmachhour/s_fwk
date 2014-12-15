## Process this file with autoreconf -i --force to create configure. -*- autoconf -*-

AC_PREREQ(2.59)

AC_INIT([s_fwk],[0.1],[opensource@spartsystems.com],[s_fwk],[http://github;com/spartsystems/s_fwk])

AC_CONFIG_SRCDIR(src/s_loop.c)

## ------------------------ ##
## Automake Initialisation. ##
## ------------------------ ##
AM_INIT_AUTOMAKE([1.11 foreign tar-pax -Wall -Werror -Wno-portability])
AM_SILENT_RULES([yes])
AM_MAINTAINER_MODE

AC_CANONICAL_HOST
#AC_CANONICAL_TARGET
AC_SUBST(PACKAGE_VERSION)

AC_C_CONST
AC_ISC_POSIX
AC_HEADER_STDC
AC_PROG_CC
AC_PROG_CC_STDC
AC_PROG_CXX
AC_PROG_CPP
AC_PROG_LN_S
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_MAKE_SET
AM_PROG_CC_C_O
AC_PROG_MKDIR_P
# Check for pkg-config manually first, as if its not installed the
# PKG_PROG_PKG_CONFIG macro won’t be defined.
# We need this for MAC OS X.
# Ref: http://www.eldemonionegro.com/blog/archivos/2007/07/25/pkg_prog_pkg_config-command-not-found
#
AC_CHECK_PROG(have_pkg_config, pkg-config, yes, no)
#
if test x”$have_pkg_config” == xno; then
AC_MSG_ERROR(pkg-config is required to install this program)
fi
#PKG_PROG_PKG_CONFIG

AC_CONFIG_HEADERS([sconfig.h])

AC_DEFINE([_GNU_SOURCE],[1],[Use GNU C library extensions (e.g. strndup).])



# Initialize libtool
LT_INIT([win32-dll])
LT_LANG([Windows Resource])

# Set some internal variables depending on the platform for later use.
AM_CONDITIONAL([AIX],    [AS_CASE([$host_os],[aix*],          [true], [false])])
AM_CONDITIONAL([ANDROID],[AS_CASE([$host_os],[linux-android*],[true], [false])])
AM_CONDITIONAL([DARWIN], [AS_CASE([$host_os],[darwin*],       [true], [false])])
AM_CONDITIONAL([FREEBSD],[AS_CASE([$host_os],[freebsd*],      [true], [false])])
AM_CONDITIONAL([LINUX],  [AS_CASE([$host_os],[linux*],        [true], [false])])
AM_CONDITIONAL([NETBSD], [AS_CASE([$host_os],[netbsd*],       [true], [false])])
AM_CONDITIONAL([OPENBSD],[AS_CASE([$host_os],[openbsd*],      [true], [false])])
AM_CONDITIONAL([SUNOS],  [AS_CASE([$host_os],[solaris*],      [true], [false])])
AM_CONDITIONAL([WINNT],  [AS_CASE([$host_os],[mingw*],        [true], [false])])




AC_CHECK_FUNCS([getloadavg poll sendmsg sendfile prctl])

AC_ARG_ENABLE(examples,
		AS_HELP_STRING([--enable-examples], [Enable the examples [default=no]]),
		[case "${enableval}" in
			yes) have_examples=true  ;;
			no) have_examples=false ;;
			*) AC_MSG_ERROR(bad value ${enableval} for --enable-examples) ;;
			esac],
		[have_examples=false])
dnl Test implies examples (for compilability test)
if test "$have_test" != "false" ; then
	have_examples=true
fi
AM_CONDITIONAL(HAVE_EXAMPLES, $have_examples)
AC_MSG_CHECKING(whether to build the examples)
if test "$have_examples" != "false" ; then
	AC_MSG_RESULT([yes])
else
	AC_MSG_RESULT([no])
fi 


AC_CONFIG_FILES([
src/Makefile
examples/Makefile
Makefile
s_fwk.pc
])

AC_OUTPUT

