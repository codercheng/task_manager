create table tbl_user (user_id int primary key auto_increment, user_name char(20) not null, user_pwd char(41) not null);


create table tbl_task (task_id bigint primary key auto_increment, user_name char(20) not null, deadline bigint not null, task_description text not null,
task_priority tinyint not null, task_state tinyint not null);  



