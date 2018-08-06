now_date=`date +%Y%m%d`
now_date=`date +"%Y-%m-%d_%H-%M-%S"`

mkdir ${now_date}
/usr/local/whistle/mysql/bin/mysqldump -uroot -B whistle --table ofuser_dlut > ./${now_date}/ofuser_dlut.sql
/usr/local/whistle/mysql/bin/mysqldump -uroot -B whistle --table organization_dlut > ./${now_date}/organization_dlut.sql
/usr/local/whistle/mysql/bin/mysqldump -uroot -B whistle --table user_organization_dlut > ./${now_date}/user_organization_dlut.sql
