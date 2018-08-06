now_date=`date +%Y%m%d`
now_date=`date +"%Y-%m-%d_%H-%M-%S"`

mkdir ${now_date}
/usr/local/whistle/mysql/bin/mysqldump -uroot data_sync > ./${now_date}/data_sync_db.sql

