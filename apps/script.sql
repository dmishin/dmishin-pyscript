create table items( id integer primary key, name);
create table instances( id integer primary key, item_id, path, media_id);
create table media( id integer primary key, mount_path, name);


