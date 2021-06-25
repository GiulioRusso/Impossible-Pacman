#include "Include.h"

Entity::Entity() :Object()
{
	moving_speed		= 1;
	dir					= LEFT;
	last_dir			= LEFT;
	next_dir			= UNKNOWN;
	animation_counter   = 0;
	death_counter		= 0;
	impossible			= false;
	isDying				= false;
	
	current_node = -1;
	dest_node = -1;


	can_right		    = true;
	can_left			= true;
	can_up				= false;
	can_down			= false;

	isMoving			= false;
	isDead				= false;

	//Definizione delle coordinate dei nodi e dei vicini
	//MATRICE DEI VICINI: vicini [nodo-1] [right=0 ; left=1 ; up=2 ; down=3] = nodo
	//1								
	vicini[0][0] = 1;
	vicini[0][1] = -1;
	vicini[0][2] = -1;
	vicini[0][3] = 6;
	
	//2								
	vicini[1][0] = 2;
	vicini[1][1] = 0;
	vicini[1][2] = -1;
	vicini[1][3] = 7;
	
	//3 							
	vicini[2][0] = -1;
	vicini[2][1] = 1;
	vicini[2][2] = -1;
	vicini[2][3] = 9;
	
	//4 		
	vicini[3][0] = 4;
	vicini[3][1] = -1;
	vicini[3][2] = -1;
	vicini[3][3] = 10;
	
	//5
	vicini[4][0] = 5;
	vicini[4][1] = 3;
	vicini[4][2] = -1;
	vicini[4][3] = 12;
	
	//6
	vicini[5][0] = -1;
	vicini[5][1] = 4;
	vicini[5][2] = -1;
	vicini[5][3] = 13;
	
	//7 
	vicini[6][0] = 7;
	vicini[6][1] = -1;
	vicini[6][2] = 0;
	vicini[6][3] = 14;
	
	//8 	
	vicini[7][0] = 8;
	vicini[7][1] = 6;
	vicini[7][2] = 1;
	vicini[7][3] = 15;
	
	//9  						
	vicini[8][0] = 9;
	vicini[8][1] = 7;
	vicini[8][2] = -1;
	vicini[8][3] = 16;
	
	//10
	vicini[9][0] = 10;
	vicini[9][1] = 8;
	vicini[9][2] = 2;
	vicini[9][3] = -1;
	
	//11 							
	vicini[10][0] = 11;
	vicini[10][1] = 9;
	vicini[10][2] = 3;
	vicini[10][3] = -1;

	//12 							
	vicini[11][0] = 12;
	vicini[11][1] = 10;
	vicini[11][2] = -1;
	vicini[11][3] = 19;
	
	//13 							
	vicini[12][0] = 13;
	vicini[12][1] = 11;
	vicini[12][2] = 4;
	vicini[12][3] = 20;
	
	//14 							
	vicini[13][0] = -1;
	vicini[13][1] = 12;
	vicini[13][2] = 5;
	vicini[13][3] = 21;
	
	//15						
	vicini[14][0] = 15;
	vicini[14][1] = -1;
	vicini[14][2] = 6;
	vicini[14][3] = -1;

	//16 							
	vicini[15][0] = -1;
	vicini[15][1] = 14;
	vicini[15][2] = 7;
	vicini[15][3] = 27;
	
	//17 					
	vicini[16][0] = 17;
	vicini[16][1] = -1;
	vicini[16][2] = 8;
	vicini[16][3] = -1;
	
	//18 						
	vicini[17][0] = -1;
	vicini[17][1] = 16;
	vicini[17][2] = -1;
	vicini[17][3] = 23;

	//19 							
	vicini[18][0] = 19;
	vicini[18][1] = -1;
	vicini[18][2] = -1;
	vicini[18][3] = 25;

	//20							
	vicini[19][0] = -1;
	vicini[19][1] = 18;
	vicini[19][2] = 11;
	vicini[19][3] = -1;

	//21						
	vicini[20][0] = 21;
	vicini[20][1] = -1;
	vicini[20][2] = 12;
	vicini[20][3] = 33;
	
	//22							 
	vicini[21][0] = -1;
	vicini[21][1] = 20;
	vicini[21][2] = 13;
	vicini[21][3] = -1;

	//23		 					
	vicini[22][0] = 23;
	vicini[22][1] = -1;
	vicini[22][2] = -1;
	vicini[22][3] = 28;

	//24							 
	vicini[23][0] = 24;
	vicini[23][1] = 22;
	vicini[23][2] = 17;
	vicini[23][3] = -1;

	//25 FANTASMA ROSSO			   	
	vicini[24][0] = 25;
	vicini[24][1] = 23;
	vicini[24][2] = -1;
	vicini[24][3] = -1;	//modificato da 31 a -1
	
	//26					
	vicini[25][0] = 26;
	vicini[25][1] = 24;
	vicini[25][2] = 18;
	vicini[25][3] = -1;

	//27				
	vicini[26][0] = -1;
	vicini[26][1] = 25;
	vicini[26][2] = -1;
	vicini[26][3] = 32;

	//28							
	vicini[27][0] = 28;
	vicini[27][1] = 33;
	vicini[27][2] = 15;
	vicini[27][3] = 38;
	
	//29							 
	vicini[28][0] = -1;
	vicini[28][1] = 27;
	vicini[28][2] = 22;
	vicini[28][3] = 34;

	//30 FANTASMA VIOLA				
	vicini[29][0] = 30;
	vicini[29][1] = -1; //-1 perch� non pu� andare a sinistra
	vicini[29][2] = 72; //anzich� -1
	vicini[29][3] = 73; //anzich� -1

	//73
	vicini[72][0] = -1;
	vicini[72][1] = -1;
	vicini[72][2] = -1;
	vicini[72][3] = 29;

	//74
	vicini[73][0] = -1;
	vicini[73][1] = -1;
	vicini[73][2] = 29;
	vicini[73][3] = -1;

	//31 FANTASMA ARANCIONE			//32		//30		//25		//0
	vicini[30][0] = 31;
	vicini[30][1] = 29;
	vicini[30][2] = 70; //anzich� 25
	vicini[30][3] = 71; //anzich� -1

	//71
	vicini[70][0] = -1;
	vicini[70][1] = -1;
	vicini[70][2] = 24;
	vicini[70][3] = 30;

	//72
	vicini[71][0] = -1;
	vicini[71][1] = -1;
	vicini[71][2] = 30;
	vicini[71][3] = -1;

	//32 FANTASMA CIANO				//0			//31		//0			//0
	vicini[31][0] = -1; //-1 perch� non pu� andare a destra
	vicini[31][1] = 30;
	vicini[31][2] = 74; //anzich� -1
	vicini[31][3] = 75; //anzich� -1

	//75
	vicini[74][0] = -1;
	vicini[74][1] = -1;
	vicini[74][2] = -1;
	vicini[74][3] = 31;

	//76
	vicini[75][0] = -1;
	vicini[75][1] = -1;
	vicini[75][2] = 31;
	vicini[75][3] = -1;

	//33							
	vicini[32][0] = 33;
	vicini[32][1] = -1;
	vicini[32][2] = 26;
	vicini[32][3] = 36;

	//34							
	vicini[33][0] = 27;
	vicini[33][1] = 32;
	vicini[33][2] = 20;
	vicini[33][3] = 43;

	//35		 					
	vicini[34][0] = 35;
	vicini[34][1] = -1;
	vicini[34][2] = 28;
	vicini[34][3] = 39;

	//36 							
	vicini[35][0] = 36;
	vicini[35][1] = 34;
	vicini[35][2] = -1;
	vicini[35][3] = -1;

	//37							
	vicini[36][0] = -1;
	vicini[36][1] = 35;
	vicini[36][2] = 32;
	vicini[36][3] = 42;

	//38							
	vicini[37][0] = 38;
	vicini[37][1] = -1;
	vicini[37][2] = -1;
	vicini[37][3] = 45;

	//39							
	vicini[38][0] = 39;
	vicini[38][1] = 37;
	vicini[38][2] = 27;
	vicini[38][3] = 47;

	//40							
	vicini[39][0] = 40;
	vicini[39][1] = 38;
	vicini[39][2] = 34;
	vicini[39][3] = -1;

	//41							
	vicini[40][0] = -1;
	vicini[40][1] = 39;
	vicini[40][2] = -1;
	vicini[40][3] = 49;

	//42							 
	vicini[41][0] = 42;
	vicini[41][1] = -1;
	vicini[41][2] = -1;
	vicini[41][3] = 50;

	//43							
	vicini[42][0] = 43;
	vicini[42][1] = 41;
	vicini[42][2] = 36;
	vicini[42][3] = -1;

	//44							 
	vicini[43][0] = 44;
	vicini[43][1] = 42;
	vicini[43][2] = 33;
	vicini[43][3] = 52;

	//45							
	vicini[44][0] = -1;
	vicini[44][1] = 43;
	vicini[44][2] = -1;
	vicini[44][3] = 54;

	//46                            
	vicini[45][0] = 46;
	vicini[45][1] = -1;
	vicini[45][2] = 37;
	vicini[45][3] = -1;

	//47                            
	vicini[46][0] = -1;
	vicini[46][1] = 45;
	vicini[46][2] = -1;
	vicini[46][3] = 56;

	//48                        
	vicini[47][0] = 48;
	vicini[47][1] = -1;
	vicini[47][2] = 38;
	vicini[47][3] = 57;

	//49                             
	vicini[48][0] = 49;
	vicini[48][1] = 47;
	vicini[48][2] = -1;
	vicini[48][3] = 58;

	//50                             
	vicini[49][0] = 69;
	vicini[49][1] = 48;
	vicini[49][2] = 40;
	vicini[49][3] = -1;

	//51                             
	vicini[50][0] = 51;
	vicini[50][1] = 69;
	vicini[50][2] = 41;
	vicini[50][3] = -1;

	//52                           
	vicini[51][0] = 52;
	vicini[51][1] = 50;
	vicini[51][2] = -1;
	vicini[51][3] = 61;
	
	//53                              
	vicini[52][0] = -1;
	vicini[52][1] = 51;
	vicini[52][2] = 43;
	vicini[52][3] = 62;

	//54                            
	vicini[53][0] = 54;
	vicini[53][1] = -1;
	vicini[53][2] = -1;
	vicini[53][3] = 63;

	//55                             
	vicini[54][0] = -1;
	vicini[54][1] = 53;
	vicini[54][2] = 44;
	vicini[54][3] = -1;

	//56                              
	vicini[55][0] = 56;
	vicini[55][1] = -1;
	vicini[55][2] = -1;
	vicini[55][3] = 65;

	//57                            
	vicini[56][0] = 57;
	vicini[56][1] = 55;
	vicini[56][2] = 46;
	vicini[56][3] = -1;

	//58                                
	vicini[57][0] = -1;
	vicini[57][1] = 56;
	vicini[57][2] = 47;
	vicini[57][3] = -1;

	//59                               
	vicini[58][0] = 59;
	vicini[58][1] = -1;
	vicini[58][2] = 48;
	vicini[58][3] = -1;

	//60                               
	vicini[59][0] = -1;
	vicini[59][1] = 58;
	vicini[59][2] = -1;
	vicini[59][3] = 66;

	//61                                
	vicini[60][0] = 61;
	vicini[60][1] = -1;
	vicini[60][2] = -1;
	vicini[60][3] = 67;
	
	//62                              
	vicini[61][0] = -1;
	vicini[61][1] = 60;
	vicini[61][2] = 51;
	vicini[61][3] = -1;

	//63                                
	vicini[62][0] = 63;
	vicini[62][1] = -1;
	vicini[62][2] = 52;
	vicini[62][3] = -1;
	
	//64                               
	vicini[63][0] = 64;
	vicini[63][1] = 62;
	vicini[63][2] = 53;
	vicini[63][3] = -1;

	//65                               
	vicini[64][0] = -1;
	vicini[64][1] = 63;
	vicini[64][2] = -1;
	vicini[64][3] = 68;

	//66                                
	vicini[65][0] = 66;
	vicini[65][1] = -1;
	vicini[65][2] = 55;
	vicini[65][3] = -1;

	//67                                
	vicini[66][0] = 67;
	vicini[66][1] = 65;
	vicini[66][2] = 59;
	vicini[66][3] = -1;

	//68                               
	vicini[67][0] = 68;
	vicini[67][1] = 66;
	vicini[67][2] = 60;
	vicini[67][3] = -1;

	//69                              
	vicini[68][0] = -1;
	vicini[68][1] = 67;
	vicini[68][2] = 64;
	vicini[68][3] = -1;

	//70 POSIZIONE DI PARTENZA DI PACMAN	
	vicini[69][0] = 50;
	vicini[69][1] = 49;
	vicini[69][2] = -1;
	vicini[69][3] = -1;

	//COORDINATE X E Y PER OGNI NODO
	//1								
	nodo[0].x = 13;
	nodo[0].y = 33;

	//2								
	nodo[1].x = 53;
	nodo[1].y = 33;

	//3 							
	nodo[2].x = 101;
	nodo[2].y = 33;

	//4 						
	nodo[3].x = 125;
	nodo[3].y = 33;

	//5  						
	nodo[4].x = 173;
	nodo[4].y = 33;

	//6 						
	nodo[5].x = 213;
	nodo[5].y = 33;

	//7 							
	nodo[6].x = 13;
	nodo[6].y = 65;

	//8 						
	nodo[7].x = 53;
	nodo[7].y = 65;

	//9  							
	nodo[8].x = 77;
	nodo[8].y = 65;

	//10							
	nodo[9].x = 101;
	nodo[9].y = 65;

	//11 							
	nodo[10].x = 125;
	nodo[10].y = 65;

	//12
	nodo[11].x = 149;
	nodo[11].y = 65;

	//13 							
	nodo[12].x = 173;
	nodo[12].y = 65;

	//14 							
	nodo[13].x = 213;
	nodo[13].y = 65;

	//15							
	nodo[14].x = 13;
	nodo[14].y = 89;

	//16 							
	nodo[15].x = 53;
	nodo[15].y = 89;

	//17 							
	nodo[16].x = 77;
	nodo[16].y = 89;

	//18 							
	nodo[17].x = 101;
	nodo[17].y = 89;

	//19 						
	nodo[18].x = 125;
	nodo[18].y = 89;

	//20							
	nodo[19].x = 149;
	nodo[19].y = 89;

	//21							
	nodo[20].x = 173;
	nodo[20].y = 89;

	//22							
	nodo[21].x = 213;
	nodo[21].y = 89;

	//23							
	nodo[22].x = 77;
	nodo[22].y = 113;

	//24							
	nodo[23].x = 101;
	nodo[23].y = 113;

	//25 FANTASMA ROSSO		
	nodo[24].x = 113;
	nodo[24].y = 113;

	//26							
	nodo[25].x = 125;
	nodo[25].y = 113;

	//27							
	nodo[26].x = 149;
	nodo[26].y = 113;

	//28							
	nodo[27].x = 53;
	nodo[27].y = 137;

	//29							
	nodo[28].x = 77;
	nodo[28].y = 137;

	//30 FANTASMA VIOLA
	nodo[29].x = 97;
	nodo[29].y = 137;

	//73
	nodo[72].x = 97;
	nodo[72].y = 131;

	//74
	nodo[73].x = 95;
	nodo[73].y = 141;

	//31 FANTASMA ARANCIONE
	nodo[30].x = 113;
	nodo[30].y = 137;

	//71
	nodo[70].x = 113;
	nodo[70].y = 131;

	//72
	nodo[71].x = 113;
	nodo[71].y = 141;

	//32 FANTASMA CIANO	
	nodo[31].x = 128;
	nodo[31].y = 137;

	//75
	nodo[74].x = 129;
	nodo[74].y = 131;

	//76
	nodo[75].x = 129;
	nodo[75].y = 141;

	//33
	nodo[32].x = 149;
	nodo[32].y = 137;

	//34						
	nodo[33].x = 173;
	nodo[33].y = 137;

	//35							 
	nodo[34].x = 77;
	nodo[34].y = 161;

	//36 PAC-MAN POSIZIONE INIZIALE
	nodo[35].x = 113;
	nodo[35].y = 161;

	//37							
	nodo[36].x = 149;
	nodo[36].y = 161;

	//38							 
	nodo[37].x = 13;
	nodo[37].y = 185;

	//39							
	nodo[38].x = 53;
	nodo[38].y = 185;

	//40
	nodo[39].x = 77;
	nodo[39].y = 185;

	//41							 
	nodo[40].x = 101;
	nodo[40].y = 185;

	//42							
	nodo[41].x = 125;
	nodo[41].y = 185;

	//43							 
	nodo[42].x = 149;
	nodo[42].y = 185;

	//44							
	nodo[43].x = 173;
	nodo[43].y = 185;

	//45							
	nodo[44].x = 213;
	nodo[44].y = 185;

	//46                        
	nodo[45].x = 13;
	nodo[45].y = 209;

	//47                               
	nodo[46].x = 29;
	nodo[46].y = 209;

	//48                             
	nodo[47].x = 53;
	nodo[47].y = 209;

	//49                               
	nodo[48].x = 77;
	nodo[48].y = 209;

	//50                          
	nodo[49].x = 101;
	nodo[49].y = 209;

	//51                          
	nodo[50].x = 125;
	nodo[50].y = 209;

	//52                                
	nodo[51].x = 149;
	nodo[51].y = 209;

	//53                               
	nodo[52].x = 173;
	nodo[52].y = 209;

	//54                               
	nodo[53].x = 197;
	nodo[53].y = 209;

	//55                              
	nodo[54].x = 213;
	nodo[54].y = 209;

	//56                              
	nodo[55].x = 13;
	nodo[55].y = 233;

	//57                             
	nodo[56].x = 29;
	nodo[56].y = 233;

	//58                               
	nodo[57].x = 53;
	nodo[57].y = 233;

	//59                             
	nodo[58].x = 77;
	nodo[58].y = 233;

	//60                                
	nodo[59].x = 101;
	nodo[59].y = 233;

	//61                              
	nodo[60].x = 125;
	nodo[60].y = 233;

	//62                              
	nodo[61].x = 149;
	nodo[61].y = 233;

	//63                                
	nodo[62].x = 173;
	nodo[62].y = 233;

	//64                             
	nodo[63].x = 197;
	nodo[63].y = 233;

	//65                               
	nodo[64].x = 213;
	nodo[64].y = 233;

	//66                       
	nodo[65].x = 13;
	nodo[65].y = 257;

	//67                            
	nodo[66].x = 101;
	nodo[66].y = 257;

	//68                           
	nodo[67].x = 125;
	nodo[67].y = 257;

	//69                            
	nodo[68].x = 213;
	nodo[68].y = 257;

	//70 POSIZIONE DI PARTENZA DI PACMAN                            
	nodo[69].x = 113;
	nodo[69].y = 209;
}
void Entity::advance()
{


}
void Entity::animate()
{
	//Incremento il contatore delle animazioni
	animation_counter++;

	//Incremento contatore animazione della morte
	if (isDying)
		death_counter++;

}
void Entity::die()
{
}
void Entity::calcoloDistMin()
{
	int infinite = 99999;
	int tmpx, tmpy;
	//Assegno valore alle distanze che conosco (nodo - vicino)
	for (int i = 0; i < 76; i++)
	{
		for (int j = 0; j < 76; j++)
			if (i == j)
				distanza[i][j] = 0;                    // Distanza da se stesso
			else if (is_near(i, j ))
			{
				tmpx = nodo[i].x - nodo[j].x;
				tmpy = nodo[i].y - nodo[j].y;

				if (tmpx != 0)
					distanza[i][j] = abs(tmpx);
				if (tmpy != 0)
					distanza[i][j] = abs(tmpy);
			}
			else
				distanza[i][j] = infinite;             //Se non sono vicini non conosco la distanza quindi la imposto a infinito
	}
	//Imposto due distanze manualmente poiche riguardano i nodi riguardanti il teletrasporto
	distanza[27][33] = 104;
	distanza[33][27] = 104;

	//Vado ad assegnare le distanze impostate a infinito di cui non conoscevo il valore,con il valore minore
	for (int k = 0; k < 76; k++)
	{
		for (int i = 0; i < 76; i++)
		{
			for (int j = 0; j < 76; j++)
			{
				if (distanza[i][k] + distanza[k][j] < distanza[i][j])
					distanza[i][j] = distanza[i][k] + distanza[k][j];
			}
		}
	}
	//Controllo se ogni nodo possiede una distanza con ogni altro nodo
	/*for (int i = 0; i < 76; i++)
	{
		for (int j = 0; j < 76; j++)
		{
			std::cout << "Distanza dal nodo[i] " << i << " al nodo[j] " << j << " e': " << distanza[i][j] << std::endl;
		}
	}*/
}
bool Entity::is_near(int i, int j)
{
	bool f = false;
	//controllo se il valore di uno dei 4 vicini e' pari a j
	for (int z = 0; z < 4; z++)
	{
		if (vicini[i][z] == j)
			f = true;
	}
	return f;
}
int Entity:: have_Near(int i,int scorr)
{
	int vicino;
	if (vicini[i][scorr] != -1)
	{
		vicino = vicini[i][scorr];
	}
	else
		vicino = -1;

	return vicino;

}