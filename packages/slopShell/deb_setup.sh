#!/usr/bin/env bash
echo "Checking installs...."
sudo apt install -y postgresql php php-pear && python -m pip install proxybroker --user
echo "Attempting to start postgresql"
if test $PG_SLOP -z; then
  echo "PGSQL Is not started, trying to init the db, and then start it."
  sudo mkdir -p /opt/postgres/slop && sudo chown -R $( whoami ):$( whoami ) /opt/postgres/slop
  SLOP_DIR="/opt/postgres/slop"
  sudo $( which chown ) $USER:postgres /opt/postgres/slop
  /usr/lib/postgresql/13/bin/pg_ctl initdb -D /opt/postgres/slop -l /opt/postgres/slop/main.log
  sudo $( which chmod ) o+w /var/run/postgresql
  sudo $( which chown ) postgres:postgres /var/run/postgresql
  /usr/lib/postgresql/13/bin/pg_ctl restart -D /opt/postgres/slop -l /opt/postgres/slop/main.log
  cd $SLOP_DIR && $(which createdb ) sloppy_bots -E utf-8 -O $( whoami ) -h localhost -p 5432
  echo "If any of these commands failed, you will need to re-run them yourself."
  echo "PG_SLOP=1" >> ~/.bashrc
  echo "export PG_SLOP" >> ~/.bashrc
  source ~/.bashrc
else
  echo "PG Appears to be running!!"
fi
