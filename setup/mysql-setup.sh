#!/bin/bash

# Default location to dump stuff and create databases
WORKDIR=`pwd`/jdbc-module-mysql

# MySQL'd "datadir"
DATADIR=${WORKDIR}/datadir

# Location for the logs
LOGDIR=${WORKDIR}/log

# Filename for download
MYSQL_PKG=mysql-5.6.12-linux-glibc2.5-x86_64
MYSQL_TGZ=${MYSQL_PKG}.tar.gz

while getopts distchw: opt
do
  case "$opt" in
        w)
            WORKDIR="$OPTARG"
            echo "Data directory: $WORKDIR"
            ;;
        d)
            echo "Download"
            mkdir -p $WORKDIR
            (
            cd $WORKDIR
            wget -c http://cdn.mysql.com/Downloads/MySQL-5.6/$MYSQL_TGZ
            )
            ;;
        i)
            echo "Install"
            (
            cd $WORKDIR
            tar -xvzf $MYSQL_TGZ
            mv $MYSQL_PKG mysql
            sudo apt-get install libaio1
            mkdir -p $DATADIR $LOGDIR $DATADIR/innodb_log
            ./mysql/scripts/mysql_install_db --user=$USER --datadir=$DATADIR --basedir=./mysql --no-defaults
            )
            MODDIR=`dirname $0`
            cp $MODDIR/my.cnf $DATADIR/my.cnf
            ;;
        s)
            echo "Start"
            (
            cd $WORKDIR/mysql
            ./bin/mysqld_safe --defaults-file=$DATADIR/my.cnf --user=$USER --pid-file=$DATADIR/mysql_rq.pid --datadir=$DATADIR --log-error=$LOGDIR/log.err &
            )
            ;;
        t)
            echo "Stop"
            kill `cat $DATADIR/mysql_rq.pid`   
            ;;
        c)
            echo "Clean everything"
            (
            cd $WORKDIR
            rm -fR ./mysql
            )
            ;;            
        h)
            echo "usage: $0 [-distcvh] [-w path]"
            echo "       -d  Download MySQL DB"
            echo "       -i  Install"
            echo "       -s  Start service"
            echo "       -t  Stop service"
            echo "       -c  Clean everything"
            echo "       -w path  Path to store everything (default ~/jdbc-module-mysql)"
            echo "       -h  Help"
            exit 2
            ;;
        *)
            if [ "$OPTERR" != 1 ] || [ "${optspec:0:1}" = ":" ]; then
                echo "Non-option argument: '-${OPTARG}'"
            fi

    esac
done
