create database blackIa;

/*création de la table personne*/
create table personne(id varchar(50) primary key, prenom varchar(50), nom varchar(50), pseudo varchar(20), sexe char(1), def varchar(100), age int, taille int,poids int, hairColor varchar(10), skinColor varchar(10), eyeColor varchar(10), birth date, ville varchar(20));

/*création de la table user*/
create table user(id varchar(50) primary key, constraint `fk_user` foreign key (id) references personne(id) on delete cascade, login varchar(20), mdp varchar(20));

/*création de la table admin*/
create table admin(admin_id varchar(50) primary key, constraint `fk_admin` foreign key (admin_id) references user(id) on delete cascade, login varchar(20), mdp varchar(20));

create table superAdmin(supAdmin_id varchar(50) primary key, constraint `fk_supAdmin` foreign key (supAdmin_id) references admin(admin_id) on delete cascade, login varchar(20), mdp varchar(20));

/*création de la table mot*/
create table mot(mot_id int unsigned auto_increment primary key, basicName varchar(20),type varchar(20), syn varchar(100), conn varchar(200), isDenm bool,*/ fun varchar(200), def varchar(100));

create table prenom(pre_id int unsigned primary key, constraint `fk_prenom` foreign key (pre_id) references mot(mot_id) on delete cascade, basicName varchar(30), isMasc bool, isFem bool);

/*création de la table verbe*/
create table verbe(verbe_id int unsigned primary key, constraint `fk_verb` foreign key (verbe_id) references mot(mot_id) on delete cascade,inf varchar(20), partPast varchar(20), partPre varchar(30), numPers int, temps varchar(20),isEtat bool, isAux bool, numGr int);

/*création de la table conjugaison*/
create table conjugaison(conj_id int unsigned auto_increment primary key, vb_id int unsigned, constraint `fk_conj` foreign key (vb_id) references verbe(verbe_id) on delete cascade,inf varchar(20), je varchar(20), tu varchar(20), il varchar(20), nous varchar(20), vous varchar(20), ils varchar(20), temps varchar(20),isEtat bool, isAux bool);

/*création de la table nom*/
create table nom(nom_id int unsigned primary key, constraint `fk_nom` foreign key (nom_id) references mot(mot_id) on delete cascade, basicName varchar(30), sing varchar(30), fem varchar(30), plur varchar(30), plurFem varchar(30), isMasc bool, isFem bool, isSing bool, isPlur bool, canMasc bool, canFem bool, canSing bool, isDenm bool);


create table adjectif(adj_id int unsigned primary key, constraint `fk_adj` foreign key (adj_id) references mot(mot_id) on delete cascade, basicName varchar(30), sing varchar(30), fem varchar(30), plur varchar(30), plurFem varchar(30), isMasc bool, isFem bool, isSing bool, isPlur bool, canMasc bool, canFem bool, canSing bool, isDenm bool);


create table determinant(det_id int unsigned primary key, constraint `fk_det` foreign key (det_id) references mot(mot_id) on delete cascade, basicName varchar(30), sing varchar(30), fem varchar(30), plur varchar(30), plurFem varchar(30), isMasc bool, isFem bool, isSing bool, isPlur bool, canMasc bool, canFem bool, canSing bool, isDenm bool);

create table pronomPers();


