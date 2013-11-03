create index index_c1_id using BTREE on c1(id);
create index index_c2_id using BTREE on c2(id);
create index index_c3_id using BTREE on c3(id);

create index index_c1_data using BTREE on c1(data(128));
create index index_c2_data using BTREE on c2(data(128));
create index index_c3_data using BTREE on c3(data(128));

create index index_facts_id1 using BTREE on facts(id1);
create index index_facts_id2 using BTREE on facts(id2);
create index index_facts_id3 using BTREE on facts(id3);

create index index_facts_id12 using BTREE on facts(id1,id2);
