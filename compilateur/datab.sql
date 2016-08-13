create database blackIa2;

/*création de la table générale des types*/
create table type(name varchar(200) primary key,cont varchar(200),descr varchar(200),meth varchar(200));



/*création de la table list_char*/
create table list_char(list_char_id int unsigned auto_increment primary key,cont varchar(2),size int);

insert into type(name,cont,descr,meth) values("list_char","list_char_id:int;size:int;","","");



/*création de la table string*/
create table string(string_id int unsigned primary key, constraint `fk_string_id` foreign key (string_id) references list_char(list_char_id) on delete cascade,size int);

insert into type(name,cont,descr,meth) values("string","string_id:list_char;size:int;","","");



/*création de la table instruction*/
create table instruction(ins_id int unsigned auto_increment primary key, name int unsigned, constraint `fk_name` foreign key (name) references string(string_id) on delete cascade,argT int unsigned, constraint `fk_argT` foreign key (argT) references string(string_id) on delete cascade,retourT int unsigned, constraint `fk_retourT` foreign key (retourT) references string(string_id) on delete cascade,cont int unsigned, constraint `fk_cont` foreign key (cont) references string(string_id) on delete cascade, prior int);

insert into type(name,cont,descr,meth) values("instruction","ins_id:int;name:string;argT:string;retourT:string;cont:string;prior:int;","","");



/*création de la table mot*/
create table mot(mot_id int primary key,basicName int unsigned, constraint `fk_basicName` foreign key (basicName) references string(string_id) on delete cascade,type int unsigned, constraint `fk_type` foreign key (type) references string(string_id) on delete cascade,syn int unsigned, constraint `fk_syn` foreign key (syn) references string(string_id) on delete cascade,conn int unsigned, constraint `fk_conn` foreign key (conn) references string(string_id) on delete cascade,isDenm bool,fun int unsigned, constraint `fk_fun` foreign key (fun) references string(string_id) on delete cascade,def int unsigned, constraint `fk_def` foreign key (def) references string(string_id) on delete cascade);

insert into type(name,descr,cont,meth) values("mot","","mot_id:int;basicName:string;type:string;syn:string;conn:string;isDenm:bool;fun:string;def:string;","");


/*création de la table prenom*/
create table prenom(pre_id int primary key, constraint `fk_pre_id` foreign key (pre_id) references mot(mot_id) on delete cascade,isMasc bool,isFem bool);

insert into type(name,descr,cont,meth) values("prenom","","pre_id:mot;isMasc:bool;isFem:bool;","");

/*création de la table verbe*/
create table verbe(verbe_id int primary key, constraint `fk_verbe_id` foreign key (verbe_id) references mot(mot_id) on delete cascade,inf int, constraint `fk_inf` foreign key (inf) references mot(mot_id) on delete cascade,partPast int, constraint `fk_partPast` foreign key (partPast) references mot(mot_id) on delete cascade,partPre int, constraint `fk_partPre` foreign key (partPre) references mot(mot_id) on delete cascade,numPers int,temps int unsigned, constraint `fk_temps` foreign key (temps) references string(string_id) on delete cascade,isEtat bool,isAux bool,numGr int);

insert into type(name,descr,cont,meth) values("verbe","","verbe_id:mot;inf:mot;partPast:mot;partPre:mot;numPers:int;temps:string;isEtat:bool;isAux:bool;numGr:int;","");


/*création de la table conjugaison*/
create table conjugaison(conj_id int primary key,verb_id int, constraint `fk_verb_id` foreign key (verb_id) references verbe(verbe_id) on delete cascade,je int, constraint `fk_je` foreign key (je) references verbe(verbe_id) on delete cascade,tu int, constraint `fk_tu` foreign key (tu) references verbe(verbe_id) on delete cascade,il int, constraint `fk_il` foreign key (il) references verbe(verbe_id) on delete cascade,nous int, constraint `fk_nous` foreign key (nous) references verbe(verbe_id) on delete cascade,vous int, constraint `fk_vous` foreign key (vous) references verbe(verbe_id) on delete cascade,ils int, constraint `fk_ils` foreign key (ils) references verbe(verbe_id) on delete cascade,tps int unsigned, constraint `fk_tps` foreign key (tps) references string(string_id) on delete cascade,isEtat bool,isAux bool);

insert into type(name,descr,cont,meth) values("conjugaison","","conj_id:int;verb_id:verbe;je:verbe;tu:verbe;il:verbe;nous:verbe;vous:verbe;ils:verbe;tps:string;isEtat:bool;isAux:bool;","");


/*création de la table nom*/
create table nom(nom_id int primary key, constraint `fk_nom_id` foreign key (nom_id) references mot(mot_id) on delete cascade,sing int, constraint `fk_sing` foreign key (sing) references mot(mot_id) on delete cascade,fem int, constraint `fk_fem` foreign key (fem) references mot(mot_id) on delete cascade,plur int, constraint `fk_plur` foreign key (plur) references mot(mot_id) on delete cascade,plurFem int, constraint `fk_plurFem` foreign key (plurFem) references mot(mot_id) on delete cascade,isMasc bool,isFem bool,isSing bool,isPlur bool,canMasc bool,canFem bool,canSing bool,isDenm bool);

insert into type(name,descr,cont,meth) values("nom","","nom_id:mot;sing:mot;fem:mot;plur:mot;plurFem:mot;isMasc:bool;isFem:bool;isSing:bool;isPlur:bool;canMasc:bool;canFem:bool;canSing:bool;isDenm:bool;","");


/*création de la table adjectif*/
create table adjectif(adj_id int primary key, constraint `fk_adj_id` foreign key (adj_id) references mot(mot_id) on delete cascade,singAdj int, constraint `fk_singAdj` foreign key (singAdj) references mot(mot_id) on delete cascade,femAdj int, constraint `fk_femAdj` foreign key (femAdj) references mot(mot_id) on delete cascade,plurAdj int, constraint `fk_plurAdj` foreign key (plurAdj) references mot(mot_id) on delete cascade,plurFemAdj int, constraint `fk_plurFemAdj` foreign key (plurFemAdj) references mot(mot_id) on delete cascade,isMasc bool,isFem bool,isSing bool,isPlur bool,canMasc bool,canFem bool,canSing bool,isDenm bool);

insert into type(name,descr,cont,meth) values("adjectif","","adj_id:mot;singAdj:mot;femAdj:mot;plurAdj:mot;plurFemAdj:mot;isMasc:bool;isFem:bool;isSing:bool;isPlur:bool;canMasc:bool;canFem:bool;canSing:bool;isDenm:bool;","");



/*création de la table determinant*/
create table determinant(det_id int primary key, constraint `fk_det_id` foreign key (det_id) references mot(mot_id) on delete cascade,singDet int, constraint `fk_singDet` foreign key (singDet) references mot(mot_id) on delete cascade,femDet int, constraint `fk_femDet` foreign key (femDet) references mot(mot_id) on delete cascade,plurDet int, constraint `fk_plurDet` foreign key (plurDet) references mot(mot_id) on delete cascade,plurFemDet int, constraint `fk_plurFemDet` foreign key (plurFemDet) references mot(mot_id) on delete cascade,isMasc bool,isFem bool,isSing bool,isPlur bool,canMasc bool,canFem bool,canSing bool,isDenm bool);

insert into type(name,descr,cont,meth) values("determinant","","det_id:mot;singDet:mot;femDet:mot;plurDet:mot;plurFemDet:mot;isMasc:bool;isFem:bool;isSing:bool;isPlur:bool;canMasc:bool;canFem:bool;canSing:bool;isDenm:bool;","");





