#!/usr/bin/env bash
SLOP_DIR="/opt/slop_shell/"
echo "Attempting to start postgresql"
if test $PG_SLOP -n; then
  echo "PGSQL Is not started, trying to init the db, and then start it."
  if ! [ -e $SLOP_DIR ];then
    sudo mkdir -p $SLOP_DIR/slop_data && sudo chown -R $( whoami ):postgres $SLOP_DIR
  else
    echo "Directory is already created."
  fi
  $( which pg_ctl ) initdb -D $SLOP_DIR/slop_data -l $SLOP_DIR/main.log
  cd $SLOP_DIR/slop_data && $( which createdb ) sloppy_bots -E utf-8 -O $( whoami ) -h localhost -p 5432 -U $( whoami )
  # shellcheck disable=SC2046
  sudo $( which chmod ) 770 /var/run/postgresql
  # shellcheck disable=SC2046
  sudo $( which chown ) postgres:postgres /var/run/postgresql
  $( which pg_ctl ) start -D $SLOP_DIR/slop_data -l $SLOP_DIR/main.log
  # shellcheck disable=SC2046
  echo "If any of these commands failed, you will need to re-run them yourself."
  echo "export PG_SLOP=1" >> ~/.zshrc
  source ~/.zshrc
else
  echo "PG Appears to be running!!"
fi