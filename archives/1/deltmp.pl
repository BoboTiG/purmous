#!/bin/perl
#
# $Author: BoboTiG <bobotig@gmail.com> (http://www.bobotig.fr)$
# $Id: deltmp$
# $Revision: 0$, $HeadURL: http://deltmp.sourceforge.net/$, $Date: 2009/11/10$
# $Revision: 0$, $Source: http://deltmp.sourceforge.net/$, $Date: 2009/11/10$
#
#
#  Delete Temporary files
#  ----------------------
#
#  This tool was written to purge temporary files on a mounted system.
#  It is full functional with:
#  - Microsoft Windows XP
#  - Microsoft Windows Vista
#  - Microsoft Windows Seven
#  - MAC & GNU/Linux systems
#  - Another OS or missing? Do not hesitate to contact me =]
#
# Under GPL v2 license.
#


#============================================================================================================
#################################### THERE YOU CAN MODIFY AS YOU FEEL #######################################
#============================================================================================================
# Languages: 0: english / 1: french
my $language = 0;



#============================================================================================================
################# KEEP AWAY FROM THIS PIECE OF CODE IF YOU DON'T KNOW WHAT YOU'RE DOING #####################
#============================================================================================================
use strict;
use warnings;
use LWP::UserAgent;
use HTTP::Request::Common qw(GET);

# Variables
my $VERSION = '0.3';	# Tool version
my $files = 0;			# Number of deleted files
my $weight = 0;		# Weight of deleted files
my $not_deleted = 0; 	# Number of not deleted files
my $file; 				# Current file
my $path;				# Specific path to solve bad issue with perl glob
my @files_to_delete; 		# Array which contains list of files to delete
my @Files;			# Array which contains all file which will be deleted
my @Folders;			# Array which contains all folders which will be deleted
my $lock = 0;			# Protection against local system deletions
my $verbose = 0;		# Verbositiy (0: none | 1: yes)
my $i = 0;
my $m = 0;

# Links for update
my $up_link = 'http://deltmp.sourceforge.net';
my $test_version = 'http://deltmp.sourceforge.net/deltmp.version';

# OS determination
my $os = $^O;
my $windows = q{};
if ( $os eq 'dos' || $os eq 'MSWin32' || $os eq 'os2' ) {
	$windows = 1;
}

# OS dependent commands
my $cmd_grep = 'grep';			# Grep command
my $cmd_del_file = 'rm -f';		# Delete single file command
my $cmd_del_folder = 'rm -rf';	# Delete directory command
my $cmd_ls = 'ls';				# Listing command
my $op_system = q{};			# Option for $cmd_ls for Windows to show system files
my $op_hidden = q{};			# Option for $cmd_ls for Windows to show hidden system files

if ( $windows ) {
	$cmd_grep = 'find';
	$cmd_del_file = 'del /F /Q';
	$cmd_del_folder = 'rmdir /S /Q';
	$cmd_ls = 'dir /B';
	$op_system = '/A:S';
	$op_hidden = '/A:H';
}

# Array for different messages
my @text;

# 0 - English
$text[0][0] = ' files found ';
$text[0][1] = 'Process to deletion? (y/n) ';
$text[0][2] = ' file deleted.'."\n";
$text[0][3] = ' files deleted.'."\n";
$text[0][4] = ' file not deleted.'."\n";
$text[0][5] = ' files not deleted.'."\n";
$text[0][6] = '[+] System purged with success!'."\n";
$text[0][7] = '[+] No file deleted.'."\n";
$text[0][8] = '[+] System not purged.'."\n";
$text[0][9] = '[+] System already purged or really clean :)'."\n";
$text[0][10] = '[+] System clean.'."\n";
$text[0][11] = ' file found ';
$text[0][13] = '[!] Path have to be absolute.'."\n";
$text[0][14] = '[!] Path have to finish by "/".'."\n";
$text[0][15] = '[-] Path: ';
$text[0][16] = '[!] Wrong OS - accepted: xp|vista|seven|linux|mac.'."\n";
$text[0][17] = '[-] OS: ';
$text[0][18] = '[+] Scanning ...'."\n";
$text[0][19] = 'Checking deltmp version ...'."\n";
$text[0][20] = '  This version:    ';
$text[0][21] = '  Lastest version: ';
$text[0][22] = '  ! Update available !'."\n";
$text[0][23] = '  ! It seems that you have a fututist version =P !'."\n";
$text[0][24] = "\n".'Go to this address to get the lastest version:'."\n";
$text[0][25] = "
Utilisation: perl deltmp.pl [OPTION] [ARGUMENTS] [-v]
	
Options:
  -h  --help		display this message
  -i  --infos		if you want know what is purged
  -p  --purge		launch the purgation
  -u  --update		update this script
  -V  --version		print tool version

Examples:
  perl deltmp.pl -p /mnt/xp/ xp
	-> purge system mounted in /mnt/xp/
	
  perl deltmp.pl -p /mnt/xp/ xp -v
	-> idem plus verbosity

deltmp version $VERSION.

";
$text[0][26] = 
"
  Delete Temporary files
  ----------------------

  This tool was written to purge temporary files on a mounted system.
  It is full functional for:
  - Microsoft Windows XP
  - Microsoft Windows Vista
  - Microsoft Windows Seven
  - MAC & GNU/Linux systems
  - Another OS or missing? Do not hesitate to contact me =]
  
  Try \"perl deltmp.pl -i (xp|vista|seven|linux|mac)\" to view details.\n";
$text[0][27] = 
"
  Delete Temporary files
  ----------------------

  == Microsoft Windows XP ==
    Folders and files to purge:
      . Documents and Settings/<*>/Local Settings/Temp/*
      . Documents and Settings/<*>/Local Settings/Temporary Internet Files/*
      . Documents and Settings/<*>/Recent/*
      . Documents and Settings/<*>/Cookies/*
          -> not index.dat
      . RECYCLER/<*>/*
      . WINDOWS/Prefetch/*
          -> not Layout.ini
      . WINDOWS/Temp/*
      . WINDOWS/\$NtUninstall*";
$text[0][28] = 
"
  Delete Temporary files
  ----------------------

  == Microsoft Windows Vista ==
    Folders and files to purge:";
$text[0][29] = 
"
  Delete Temporary files
  ----------------------

  == Microsoft Windows Seven ==
    Folders and files to purge:
      . \$Recycle.Bin/<*>/*
      . Windows/Prefetch/*
          -> not Layout.ini
      . Windows/Temp/*";
$text[0][30] = 
"
  Delete Temporary files
  ----------------------

  == MAC & GNU/Linux based system ==
    Folders and files to purge:
      . tmp/*
      . var/tmp/*";
$text[0][31] = "\n\n".'     >> A missing? Do not hesitate to contact me <<'
		."\n\n".'Lengend: <*> all users | * all files and folders'."\n";


# 1 - French
$text[1][0] = ' fichiers trouvés ';
$text[1][1] = 'Lancer la suppression ? (y/n) ';
$text[1][2] = ' fichier supprimé.'."\n";
$text[1][3] = ' fichiers supprimés.'."\n";
$text[1][4] = ' fichier non supprimé.'."\n";
$text[1][5] = ' fichiers non supprimés.'."\n";
$text[1][6] = '[+] Système purgé avec succès !'."\n";
$text[1][7] = '[+] Aucun fichier supprimé.'."\n";
$text[1][8] = '[+] Système non purgé.'."\n";
$text[1][9] = '[+] Système déjà purgé ou vraiment propre :)'."\n";
$text[1][10] = '[+] Système propre.'."\n";
$text[1][11] = ' fichier trouvé ';
$text[1][13] = '[!] Le chemin doit être absolu.'."\n";
$text[1][14] = '[!] Le chemin doit finir par "/".'."\n";
$text[1][15] = '[-] Chemin : ';
$text[1][16] = '[!] Mauvais SE - acceptés : xp|vista|seven|linux|mac.'."\n";
$text[1][17] = '[-] SE : ';
$text[1][18] = '[+] Scan en cours...'."\n";
$text[1][19] = 'Vérification de la version de deltmp...'."\n";
$text[1][20] = '  Version actuelle :    ';
$text[1][21] = '  Dernière version : ';
$text[1][22] = '  ! Mise à jour disponible !'."\n";
$text[1][23] = '  ! Il semblerait que tu aies une versions futuriste =P !'."\n";
$text[1][24] = "\n".'Rendez-vous à l\'adresse suivante pour récupérer la dernière version :'."\n";
$text[1][25] = "
Utilisation : perl deltmp.pl [OPTION] [ARGUMENTS] [-v]
	
Options :
  -h  --help		affiche cette aide
  -i  --infos		si vous voulez savoir ce qui est purgé
  -p  --purge		procédure de purgation
  -u  --update		mettre à jour ce script
  -V  --version		affiche la version du l'utilitaire

Exemples :
  perl deltmp.pl -p /mnt/xp/ xp
	-> purge le système monté dans /mnt/xp/
	
  perl deltmp.pl -p /mnt/xp/ xp -v
	-> idem plus verbosité

deltmp version $VERSION.

";
$text[1][26] = 
"
  Supprimer les fichiers temporaires
  ----------------------------------

  Cet utilitaire a été écrit pour purger les fichiers temporaires d'un système monté.
  Il est pleinement fonctionnel pour :
  - Microsoft Windows XP
  - Microsoft Windows Vista
  - Microsoft Windows Seven
  - MAC & systèmes basés GNU/Linux
  - Autre SE ou un oubli ? N'hésitez pas à me contacter =]
  
  Essayez \"perl deltmp.pl -i (xp|vista|seven|linux|mac)\" pour plus de détails.\n";
$text[1][27] = 
"
  Supprimer les fichiers temporaires
  ----------------------------------

  == Microsoft Windows XP ==
    Fichiers et dossier à purger :
      . Documents and Settings/<*>/Local Settings/Temp/*
      . Documents and Settings/<*>/Local Settings/Temporary Internet Files/*
      . Documents and Settings/<*>/Recent/*
      . Documents and Settings/<*>/Cookies/*
          -> sauf index.dat
      . RECYCLER/<*>/*
      . WINDOWS/Prefetch/*
          -> sauf Layout.ini
      . WINDOWS/Temp/*
      . WINDOWS/\$NtUninstall*";
$text[1][28] = 
"
  Supprimer les fichiers temporaires
  ----------------------------------

  == Microsoft Windows Vista ==
    Fichiers et dossier à purger :";
$text[1][29] = 
"
  Supprimer les fichiers temporaires
  ----------------------------------

  == Microsoft Windows Seven ==
    Fichiers et dossier à purger :
      . \$Recycle.Bin/<*>/*
      . Windows/Prefetch/*
          -> not Layout.ini
      . Windows/Temp/*";
$text[1][30] = 
"
  Supprimer les fichiers temporaires
  ----------------------------------

  == MAC & systèmes GNU/Linux ==
    Fichiers et dossier à purger :
      . tmp/*
      . var/tmp/*";
$text[1][31] = "\n\n".'     >> Un oubli ? N\'hésitez pas à me contacter <<'
		."\n\n".'Légende : <*> tous les utilisateurs | * tous les fichiers et dossiers'."\n";

#
# Object: check path sub.
#
sub check_path {
	my @argz = @_;
	
	# Does the path is absolute and finish by "/"?
	if ( !$argz[0] ) {
		die($text[$language][25]);
	} elsif ( substr($argz[0], 0, 1) ne '/' ) {
		die($text[$language][13]);
	} elsif ( substr($argz[0], -1) ne '/' ) {
		die($text[$language][14]);
	} else {
		print $text[$language][15].$argz[0]."\n";
	}
	
	# Local system?
	$lock = 1 if ( $argz[0] eq '/' );
	
	return;
}

#
# Object: count files in one folder, recursive function.
#
sub count {
	my @argz = @_;
	
	opendir(DIR, $argz[0]);
	while ( my $f = readdir(DIR) ) {
		if ( $f ne '.' && $f ne '..' ) {
			count_file($argz[0].'/'.$f);
		}
	}
}

#
# Object: store files and folders and retieves weight file.
#
sub count_file {
	my @argz = @_;
	
	print $argz[0]."\n" if ( $verbose );
	
	++$files;

	if ( -d "$argz[0]" ) {
		$Folders[$m] = "$argz[0]";
		++$m;
		count("$argz[0]");
	} else {
		$Files[$i] = "$argz[0]";
		++$i;
		$weight += (stat($argz[0]))[7];
	}
	
	return;
}

#
# Object: sub to ask and delete files and folders.
#
sub delete_all_files {
	my @argz = @_;
	my $rep;
	my $file_to_del;
	my $folder_to_del;
	my $j = 0;
	
	if ( $files > 1 ) {
		print "\t-> ".$files.$text[$language][0].weight()."\n";
	} else {
		print "\t-> ".$files.$text[$language][11].weight()."\n";
	}
	return if ( $lock );
	
	if ( $files > 0 ) {
		print "\t-> ".$text[$language][1];
		chomp($rep = <STDIN>);
		
		if ( $rep eq 'y' || $rep eq 'Y' ) {
			foreach $file_to_del ( @Files ) {
				if ( `$cmd_del_file "$file_to_del"` ) {
					++$not_deleted;
				} else {
					++$j;
				}
			}

			foreach $folder_to_del ( @Folders ) {
				if ( !`$cmd_del_folder "$folder_to_del"` ) {
					++$not_deleted;
				} else {
					++$j;
				}
			}
		
			if ( $not_deleted == 1 ) {
				print '[!] 1'.$text[$language][4];
			} elsif ( $not_deleted > 1 ) {
				print '[!] '.$not_deleted.$text[$language][5];
			}

			if ( $j == 1 ) {
				print '[+] 1'.$text[$language][2];
			} elsif ( $j > 1 ) {
				print '[+] '.$j.$text[$language][3];
			}
		
			print $text[$language][6];
		} else {
			print $text[$language][7];
			print $text[$language][8];
		}
	} else {
		print $text[$language][9];
	}

	return;
}

#
# Object: after deletion, print information.
#
sub files_deleted {
	my @argz = @_;
	
	if ( $argz[0] == 0 ) {
		print $text[$language][10];
	} elsif ( $argz[0] == 1 ) {
		print '[+] 1'.$text[$language][2];
	} else {
		print '[+] '.$argz[0].$text[$language][3];
	}
	return;
}

#
# Object: main sub, arguments traitment.
#
sub main {
	my @argz = @_;
	
	# Arguments traitment
	if ( $argz[0] ) {
		if ( $argz[0] eq '-h' || $argz[0] eq '--help' ) {
			die($text[$language][25]);
		} elsif ( $argz[0] eq '-i' || $argz[0] eq '--infos' ) {
			print '>>> deltmp v.'.$VERSION."\n";
			
			# OS specifications
			if ( $argz[1] ) {
				if ( $argz[1] eq 'xp' ) {
					print $text[$language][27];
					print $text[$language][31];
				} elsif ( $argz[1] eq 'vista' ) {
					print $text[$language][28];
					print $text[$language][31];
				} elsif ( $argz[1] eq 'seven' ) {
					print $text[$language][29];
					print $text[$language][31];
				} elsif ( $argz[1] eq 'linux' || $argz[1] eq 'mac' ) {
					print $text[$language][30];
					print $text[$language][31];
				}  else {
					print $text[$language][26];
				}
			}  else {
				print $text[$language][26];
			}
		} elsif ( $argz[0] eq '-p' || $argz[0] eq '--purge' ) {
			print '>>> deltmp v.'.$VERSION."\n";

			# Check path
			check_path($argz[1]);
			
			# Verbose?
			$verbose = 1 if ( $argz[3] && $argz[3] eq '-v' );
			
			# OS specifications
			if ( $argz[2] ) {
					if ( $argz[2] eq 'xp' ) {
					purge_xp($argz[1]);
				} elsif ( $argz[2] eq 'vista' ) {
					#purge_vista($argz[0]);
				} elsif ( $argz[2] eq 'seven' ) {
					#purge_seven($argz[0]);
				} elsif ( $argz[2] eq 'linux'|| $argz[2] eq 'mac' ) {
					purge_linux_mac($argz[1]);
				} else {
					die($text[$language][16]);
				}
			} else {
				die($text[$language][16]);
			}
			
			return;
		} elsif ( $argz[0] eq '-V' || $argz[0] eq '--version' ) {
			version();
		} elsif ( $argz[0] eq '-u' || $argz[0] eq '--update' ) {
			update();
		} else {
			die($text[$language][12]);
		}
	}
	
	return;
}

#
# Object: MAC & GNU/Linux purge sub.
#
sub purge_linux_mac {
	my @argz = @_;

	# === MAC & GNU/Linux based systems ===
	#    Folders and files to purge:
	#    - tmp/*
	#    - var/tmp/*
	#
	
	print $text[$language][17].'MAC & GNU/Linux'."\n"; 
	print $text[$language][18];
	
	# tmp/*
	@files_to_delete = <"$argz[0]tmp/*">;
	foreach $file ( @files_to_delete ) {
		count_file("file");
	}
	
	# var/tmp/*
	@files_to_delete = <"$argz[0]var/tmp/*">;
	foreach $file ( @files_to_delete ) {
		count_file("$file");
	}
	
	# Ask for deletion
	delete_all_files();
	return;
}

#
# Object: Microsoft Windows XP purge sub.
#
sub purge_xp {
	my @argz = @_;
	
	# == Microsoft Windows XP ==
	#    Folders and files to purge:
	#    - Documents and Settings/<*>/Local Settings/Temp/*
	#    - Documents and Settings/<*>/Local Settings/Temporary Internet Files/*
	#    - Documents and Settings/<*>/Recent/*
	#    - Documents and Settings/<*>/Cookies/*
	#    - RECYCLER/<*>/*
	#    - WINDOWS/Prefetch/*
	#    - WINDOWS/Temp/*
	#    - WINDOWS/$NtUninstall*
	#

	print $text[$language][17].'Microsoft Windows XP'."\n";
	print $text[$language][18];

	# Documents and Settings/<*>
	my @users = <"$argz[0]Documents\ and\ Settings/*">;
	foreach my $user ( @users ) {
		my @user = split('/', $user);
		$user = $user[$#user];
		
		if ( $user ne 'All Users' && $user ne 'Default User' && $user ne 'LocalService' && $user ne 'NetworkService' ) {
			# (...)/Local Settings/Temp/*
			@files_to_delete = <"$argz[0]Documents\ and\ Settings/$user/Local Settings/Temp/*">;
			foreach $file ( @files_to_delete ) {
				count_file("$file");
			}

			# (...)/Local Settings/Temporary Internet Files/*
			@files_to_delete = <"$argz[0]Documents\ and\ Settings/$user/Local Settings/Temporary Internet Files/*">;
			foreach $file ( @files_to_delete ) {
				count_file("$file");
			}

			# (...)/Recent/*
			@files_to_delete = <"$argz[0]Documents\ and\ Settings/$user/Recent/*">;
			foreach $file ( @files_to_delete ) {
				count_file("$file");
			}

			# (...)/Cookies/*
			@files_to_delete = <"$argz[0]Documents\ and\ Settings/$user/Cookies/*">;
			foreach $file ( @files_to_delete ) {
				# Do not delete index.dat!
				my @filen = split('/', $file);
				my $filen = $filen[$#filen];
				
				if ( $filen ne 'index.dat' ) {
					count_file("$file");
				}
			}
		}
	}

	# RECYCLER/<*>/*
	$path = "$argz[0]RECYCLER/";
	my @recyclers = `$cmd_ls $op_system "$path"`;
	foreach my $recycler ( @recyclers ) {
		chop($recycler);
		@files_to_delete = `$cmd_ls "$path$recycler/"`;
		foreach $file ( @files_to_delete ) {
			chop($file);
			count_file($path.$recycler.'/'.$file);
		}
	}
	
	# WINDOWS/Prefetch/*
	$path = "$argz[0]WINDOWS/Prefetch/";
	@files_to_delete = `$cmd_ls "$path"`;
	foreach $file ( @files_to_delete ) {
		# Do not delete Layout.ini!
		chop($file);
		if ( $file ne 'Layout.ini' ) {
			count_file($path.$file);
		}
	}

	# WINDOWS/Temp/*
	$path = "$argz[0]WINDOWS/Temp/";
	@files_to_delete = `$cmd_ls "$path"`;
	foreach $file ( @files_to_delete ) {
		chop($file);
		count_file($path.$file);
	}

	# WINDOWS/$NtUninstall*
	$path = "$argz[0]WINDOWS/";
	@files_to_delete = `$cmd_ls $op_hidden "$path" | $cmd_grep "\\\$NtUninstall"`;
	foreach $file ( @files_to_delete ) {
		chop($file);
		count_file($path.$file);
	}

	# Ask for deletion
	delete_all_files();
	return;
}

#
# Object: Microsoft Windows XP scan sub.
#
sub scan_xp {
	my @argz = @_;
	
	# == Microsoft Windows XP ==
	#    Files to find:
	#    - .~*
	#

	print $text[$language][17].'Microsoft Windows XP'."\n";
	print $text[$language][18];

	@files_to_delete = <"$argz[0]*">;
	foreach $file ( @files_to_delete ) {
		count_file_scan("$file", '.~');
	}

	# Ask for deletion
	delete_all_files();
	return;
}

#
# Object: update operations.
#
sub update {
	print $text[$language][19];
	
	my $agent = new LWP::UserAgent;
	my $requete = $agent->request(new HTTP::Request('GET', $test_version));
	my $lastest = $requete->content;
	
	print $text[$language][20].$VERSION."\n";
	print $text[$language][21].$lastest;
	
	if ( $VERSION < $lastest ) {
		print $text[$language][22];
		print $text[$language][24];
		print $up_link."\n";
	} elsif ( $VERSION > $lastest ) {
		print $text[$language][23];
	}
	
	return;
}

#
# Object: print tool version (for external use).
#
sub version {
	print $VERSION;
	exit;
}

#
# Object: print total weight of futur deleted files and folders.
#
sub weight {
	my @argz = @_;
	
	if ( $weight > 1000000000 ) {
		return sprintf("(%0.3f Go)", ($weight / 1024 / 1024 / 1024));
	} elsif ( $weight > 1000000 ) {
		return sprintf("(%0.3f Mo)", ($weight / 1024 / 1024));
	} elsif ( $weight == 1000 ) {
		return sprintf("(%0.3f Ko)", ($weight / 1024));
	} else {
		return sprintf("(%.0f o)", $weight);
	}
}

# Let's go!
main(@ARGV);

exit;
