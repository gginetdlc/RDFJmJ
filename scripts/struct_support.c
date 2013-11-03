/*************************************************************************/
/*  struct_support.c                                                     */
/*  author: Jose Miguel Noblecilla                                       */
/*  created: 01/11/2013                                                  */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*  This program is free software: you can redistribute it and/or modify */
/*  it under the terms of the GNU General Public License as published by */
/*  the Free Software Foundation, either version 3 of the License, or    */
/*  (at your option) any later version.                                  */
/*                                                                       */
/*  This program is distributed in the hope that it will be useful,      */ 
/*  but WITHOUT ANY WARRANTY; without even the implied warranty of       */
/*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the        */
/*  GNU General Public License for more details.                         */
/*                                                                       */
/*  You should have received a copy of the GNU General Public License    */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/> */
/*************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <sys/time.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include "md5.h"

char NAME_TRIPLETS[20] = "tmp/triplets";
char NAME_TRIPLETS_NODIR[20] = "triplets";
char LOG_FORMAT[40] = "logs/%s-%s.log";
const int CHUNK_SIZE = 1000;
const int MAXLEN_COL = 100005;

char *curdate() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80], *ans;
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
	ans = (char *)malloc( strlen(buf) );
	strcpy( ans, buf );
    return ans;
}

FILE *init_log( char *source ){
	char tname[40];
	sprintf(tname,LOG_FORMAT, source, curdate());
	return fopen( tname, "w+");
}

char *addslashes(char *s){
	char p[ strlen(s) * 4 ];
	char *t = &p[0];
	int len = 0;
	while ( *s ){
		if (*s == '\"' || *s == '\'' )	*t = '\\', t++, len++;
		*t = *s;
		t++,s++;
		len++;
	}
	*t = 0;
	char *real_t = (char *)malloc( len + 1 );
	strcpy( real_t, p );
	return real_t;
	
}


void generate_triplets( FILE *log, char *file_name ){
	time_t start, stop;
    clock_t ticks; long count;
	char command[100];
	sprintf( command, "cat %s | awk '{out=$1; for(i=2;i<NF-1;i++){out=out\" \"$i}; print out}' > %s", file_name, NAME_TRIPLETS);
//	printf("%s\n",command);
	time(&start);
	fprintf(log,"PRE-PROCESS TRIPLETS: Started\n", difftime(stop, start));
	fprintf(log,"\tCommand executed: %s\n", command );
	system(command);
	time(&stop);
	fprintf(log, "PRE-PROCESS TRIPLETS: Finished in about %.2f seconds. \n\n", difftime(stop, start));
}

void insertTrip(){
	char h1[33],h2[33],h3[33];
	char name_out[100] = "SQL-Generated/insert-";
	strcat(name_out,NAME_TRIPLETS_NODIR);
	strcat(name_out,".sql");
	FILE *fcol = fopen(NAME_TRIPLETS,"r"), *fout = fopen(name_out,"w") ;
	char line[ 10005 ];
	int pos = 0;
	fprintf(fout,"INSERT facts VALUES " );
	while( fgets(line, sizeof line ,fcol ) != NULL ){
		int len = strlen(line);
		if ( len == 0 ) continue;
		if ( pos ) fprintf(fout, ",");
		fprintf(fout,"\n");

		char *token1 = addslashes(strtok(line," ")),
		 *token2 = addslashes(strtok(NULL," ")),
		 *token3 = addslashes(strtok(NULL,"\n"));

		MD5(token1, h1); MD5(token2, h2); MD5(token3, h3);

		fprintf(fout,"(unhex('%s'), unhex('%s'), unhex('%s') )",h1,h2,h3 );
		if ( pos == CHUNK_SIZE ){
			pos = -1;
			fprintf(fout,";\n");
			fprintf(fout,"INSERT facts VALUES");
		}
		pos++;
		
	}
	fprintf(fout,";");
	fclose(fcol);
	fclose(fout);
}
void insertCol( char *name_col, char *name_col_nodir, int col ){
	char name_out[100] = "SQL-Generated/insert-";
	strcat(name_out,name_col_nodir);
	strcat(name_out,".sql");
	puts(name_out);
	FILE *fcol = fopen(name_col,"r"), *fout = fopen(name_out,"w") ;
	char line[ MAXLEN_COL ], h[33];
	int pos = 0;
	fprintf(fout,"INSERT c%d VALUES ", col);
	while( fgets(line, sizeof line ,fcol ) != NULL ){
		int len = strlen(line);
		if ( len == 0 ) continue;
		line[len-1] = 0; // remove the '\n'
		if ( pos ) fprintf(fout, ",");
		fprintf(fout,"\n");
		
		MD5(line, h);
	
		fprintf(fout,"(unhex('%s'), '%s')",h, addslashes(line));
		if ( pos == CHUNK_SIZE ){
			pos = -1;
			fprintf(fout,";\n");
			fprintf(fout,"INSERT c%d VALUES", col);
		}
		pos++;
		
	}
	fprintf(fout,";");
	fclose(fcol);
	fclose(fout);
}

void get_column( FILE *log, int col, int multiple ){
	time_t start, stop;
    clock_t ticks; long count;
	char command[100], name_col[100], name_col_nodir[100];
	int fd[2], rows;

	sprintf(name_col, "%s-%d", NAME_TRIPLETS, col);
	sprintf(name_col_nodir, "%s-%d", NAME_TRIPLETS_NODIR, col);
	if (! multiple ){
		sprintf( command, "cat %s | awk '{print $%d}' | sort -u > %s", NAME_TRIPLETS, col ,name_col);
	}else{
		sprintf( command, "cat %s | awk '{out=$%d; for(i=%d;i<=NF;i++){out=out\" \"$i}; print out}' | sort -u > %s", NAME_TRIPLETS, col, col + 1, name_col);
	}
	time(&start);
	fprintf(log,"CREATING UNIQUE COLUMNS: Started\n", difftime(stop, start));
	fprintf(log,"\tCommand executed: %s\n", command );
	system(command);
	pipe(fd);
	dup2( fd[1], 1 );
	sprintf(command, "wc -l %s", name_col);
	system(command);
	read( fd[0] , &rows, sizeof(rows));
	fprintf(log,"\tTotal rows result: %d\n", rows );
	fprintf(log,"\tProcessing inserts + hashes\n");
	
	insertCol( name_col,name_col_nodir, col );

	time(&stop);
	fprintf(log, "CREATING UNIQUE COLUMNS: Finished in about %.2f seconds. \n\n", difftime(stop, start));
}
