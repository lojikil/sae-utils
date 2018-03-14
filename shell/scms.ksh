#!/bin/sh
# Souce Code Management System
# Copyright 2003-2006 Stefan Edwards.
# Released under the BSD license
# Change log:
# 0.1 : initial release
# 0.2 : cleaned, added 'lsproj', 'lsbranch' & 'commit'
# 0.3 : fixed the usage
# 0.4 : changed the giant if block to a clean case
# 0.4a: fixed the diff output from constantly showing while commiting
# 0.4b: added length checking to arguments
# 0.4c: fixed the bug that init would ignore a path passed to it
#	*NOTE* still needs either "." or absolute path!
# 0.5 : added a 'release' function, creates a tarball of the project
# 0.5a: added a 'tag' function, similar to CVS tags
# To Do:
#     - Diffs: need to support diffs & partial stores to reduce space
#     - Update: after supporting diffs, support merges against CWD
#     - Root: change the {root} to root for ease of tagging/branching
do_start ()
{
	cd $HOME
	if [ -d .scms ]
	then
		echo "You have already started scms"
	else
		echo "Create a .scms directory in $HOME"
		mkdir .scms
	fi
	echo "Done"
}
do_init ()
{
	if [ $2 = "." ]; then
		origdir=`pwd`	
	else
		origdir=$2
	fi	
	cd $HOME/.scms
	if [ -d $1 ]
	then
		echo "You have already initialized project" $1
	else
		mkdir $1
		mkdir $1/\{root\}	
		cd $1/\{root\}
		cp -R $origdir/* .	
	fi
	echo "Finished setting up directory structure & importing files"
}
do_co ()
{
	local locatedir=`pwd`	
	cd $HOME/.scms
	if [ -d $1 ] 
	then
		cd $1	
		if [ $2 = "root" ]
		then
			cd \{root\}	
			cp -R ./* $locatedir
		elif [ -d $2 ]
		then
			cd $2	
			cp -R ./* $locatedir
		else
			echo "The specified branch was not found!"
		fi
	else
		echo "No such project name!"
	fi
}
do_ci()
{
	local origdir=`pwd`	
	cd $HOME/.scms
	if [ -d $1 ]
	then
		cd $1
		if [ $2 = "root" ] 
		then 
			cd \{root\}
		elif [ -d $2 ]
		then
			cd $2
		else
			echo "No Such branch:" $2
			exit
		fi	
		for i in * 
		do
			diff $origdir/$i $i > /dev/null 
			if [ $? -eq 1 ]
			then
				cp $origdir/$i $i
			fi
		done
	else
		echo "No such project"
	fi
}	
do_branch ()
{
	cd $HOME/.scms
	if [ -d $1 ]
	then
		cd $1
		if [ $2 = "root" ]
		then
			mkdir $3
			cp -R \{root\}/* $3
		elif [ -d $2 ]
		then
			mkdir $3
			cp -R $2/* $3
		else
			echo "No such revision" $2
		fi
	else
		echo "No such project" $1
	fi
}
do_release ()
{
	origdir=`pwd`	
	if [ -d $HOME/.scms/$1 ]; then	
		if [ -d $HOME/.scms/$1/$2 ]; then	
			if [ ! -d $HOME/tmp ]; then
				mkdir $HOME/tmp
			fi
			mkdir $HOME/tmp/$1-$2
			cp -R $HOME/.scms/$1/$2/* $HOME/tmp/$1-$2
			cd $HOME/tmp	
			tar -cvf $1-$2.tar $1-$2
			gzip $1-$2.tar
			cp $1-$2.tar.gz $origdir
		else
			echo "No such branch:" $2
		fi
	else
		echo "No such project:" $1
	fi
}
do_tag ()
{
	if [ -d $HOME/.scms ]; then
	 if [ -d $HOME/.scms/$1 ]; then
	  if [ -d $HOME/.scms/$1/$2 ]; then
	    ln -s $HOME/.scms/$1/$2 $HOME/.scms/$1/$3
	  else
	    echo "No such revision"
	  fi
	 else
	    echo "No such project"
	 fi
	else
	    echo "Run start before anything else"
	fi
}	
case ${1:-"usage"} in
	"start")
		do_start ;;
	"init") 
	 	if [ $# -ne 3 ]; then
			echo "Usage: init project path"
			exit
		fi
		do_init $2 $3 ;;
	"commit")
		if [ $# -ne 3 ]; then
			echo "Usage: commit project revision"
			exit
		fi	
		do_ci $2 $3 ;;
	"import")
		echo "Import will, at a later date, import from a remote repos"
		;;
	"checkout")
		if [ $# -ne 3 ]; then
			echo "Usage: checkout project revision"
			exit
		fi	
		do_co $2 $3 ;;
	"tag")
		if [ $# -ne 4 ]; then
			echo "Usage: tag project revision tag"
			exit
		fi
		do_tag $2 $3 $4
		;;	
	"update")
		echo "update is not yet made, as Diffs are not yet supported"
		;;	
	"branch")
		if [ $# -ne 4 ]; then
			echo "Usage: branch project revision newrevision"
			exit
		fi	
		do_branch $2 $3 $4 ;;
	"diff")
		echo "Diff is not yet implemented" ;;
	"lsproj")
		if [ -d $HOME/.scms ]; then
			ls $HOME/.scms
		else
			echo "You must use start before lsproj"
		fi
		;;
	"release")
		if [ $# -ne 3 ]; then
			echo "Usage: release project revision"
			exit
		fi
		do_release $2 $3
		;;	
	"lsbranch")
		if [ $# -ne 2 ]; then
			echo "Usage: lsbranch projectname"
			exit
		fi
		if [ -d $HOME/.scms ]; then
			if [ -d $HOME/.scms/$2 ]; then
				ls $HOME/.scms/$2
			else
				echo "No such project"
			fi
		else
			echo "You must use start before lsbranch"
		fi
		;;
	"usage")
		cat << EOUSAGE
Source Code Management System
-----------------------------
usage: scms command 
commands:
start | init | checkout | commit | import | branch | diff | lsproj | lsbranch
| release
Summary: 
	A simple SCM written as a ksh shell script. Does not do real diffs
yet, and it is not really meant for multiple developers, but is fine for
those of us who simply need to have small repositories of code & data
managed quickly.
Example usage:
$ scms start # start the repository
$ scms init project path
$ scms branch project revision
$ cd /to/some/path
$ scms checkout project revision
$ vi somefile
$ scms commit project revision
$ scms lsproj
project
$ scms lsbranch project
{root} revision
$ scms release project revision
$ ls
project-revision.tar.gz
EOUSAGE
	;;	
	*)
		echo "Unknown Command:" $1
	;;
esac
