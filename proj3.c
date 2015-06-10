/*
 * Soubor:  proj3.c
 * Datum:   2013/12/15
 * Autor:   Martin Jurik, xjurik08@stud.fit.vutbr.cz
 * Projekt: Hledani obrazcu, projekt c. 3 pro predmet IZP
 * Popis:   viz. --help
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct
{
    int rows;
    int cols;
    char *cells;//cells samotne pole s daty
} Bitmap;

typedef struct
{
    int x1,x2,y1,y2,plocha,vzdalenost;
} Square;

void help()
{
    printf (
        "Program v danem monochromatickem obrazku hleda nejdelsi vodorovne a svisle\n"
        "usecky a nejvetsi ctverce. Obrazek je ulozen jako bitmapa v textovem souboru\n"
        "ve forme obdelnikove matice nul a jednicek. Cilem programu je vypis pocatecni\n"
        "a koncove souradnice nalezene usecky nebo ctverce.\n"
        "   Argumenty programu:\n"
        "      --help          - program vytiskne napovedu pouzivani programu a skonci \n"
        "      --test          - pouze zkontroluje, ze soubor dany druhym argumentem programu \n"
        "                        obsahuje radnou definici bitmapoveho obrazku. V pripade, ze \n"
        "                        format obrazku odpovida definici, vytiskne Valid. V opacnem \n"
        "                        pripade (ilegalnich znaku, chybejicich udaju,spatnych hodnot) \n"
        "                        program tiskne Invalid.\n"
        "      --hline         - v danem obrazku nalezne a vytiskne pocatecni a koncovou \n"
        "                        souradnici prvni nejdelsi horizontalni usecky. \n"
        "      --vline         - v danem obrazku nalezne a vytiskne pocatecni a koncovou \n"
        "                        souradnici prvni nejdelsi vertikalni usecky. \n"
        "      --square        - v danem obrazku nalezne a vytiskne pocatecni a koncovou \n"
        "                        souradnici prvniho nejvetsiho ctverce. \n"
    );
}

char* alloc2D(Bitmap*bitmap) //pointer na bitmapu ktery se jmenuje bitmap
{
    bitmap->cells=malloc((bitmap->rows*bitmap->cols)*sizeof(char));
    if (bitmap->cells == NULL)
    {
        printf("Chyba alokace\n");
        return NULL;
    }
    return bitmap->cells;
}

char getcolor(Bitmap*bitmap,int x,int y)
{
    return bitmap->cells[y*bitmap->cols +x]; // saham na místo v dane matici (poli)
}

int readArray2D(Bitmap*bitmap,FILE* soubor)
{
    for (int i=0; i<bitmap->rows; i++) //prochazim radky a sloupce
    {
        for (int j=0; j<bitmap->cols; j++)
        {
            if (fscanf(soubor,"%d ",(int *)&bitmap->cells[i*bitmap->cols+j]) == 0) // pole charu, vracim inty ( cisla ) proto castovani (int *), pokud najdu na policku nesrovnalost tak error 1
            {
                return 1;
            }
            if((bitmap->cells[i*bitmap->cols+j] != 0) && (bitmap->cells[i*bitmap->cols+j]!=1))
            {
                return 1;
            }
        }
    }
    return 0;
}

int find_vline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2)
{
    int delka, sloupce, radky, zx, zy, kx, ky, pomzx, pomzy, pomkx, pomky, pomdel;
    pomdel = delka = zx = zy = kx = ky = pomzx = pomzy = pomkx = pomky = 0;

    for(sloupce = 0; sloupce < bitmap->cols; sloupce++)
    {
        zy = ky = sloupce; // zacatecni a koncove y
        delka = 0;
        for(radky = 0; radky < bitmap->rows; radky++) //prochazim vsechny radky v ramci jednoho sloupce
        {
            if(getcolor(bitmap,sloupce,radky)==1)
            {
                if(delka==0)  // resim - nemam stanoveny pocatecni x-ove souradnice
                {
                    zx = kx = radky;
                }
                else
                {
                    kx = radky; //nasel sem 1cku na dalsim radku
                }
                delka++; // tak ci tak, aspon 1 jednicku jsem nasel, tak zvysim
            }
            else // kdyz najdu nulu
            {
                if(delka!=0)
                {
                    if(delka>pomdel) //soucasna delka je vetsi nez pomocna (znaci mi nejdelsi primku)
                    {
                        pomzx=zx;
                        pomzy=zy;
                        pomkx=kx;
                        pomky=ky;
                        pomdel=delka;
                    }
                    delka=0; // pak vynuluju, nasel jsem 0 a konci mi primka
                }
            }

            if(delka>pomdel)
            {
                pomzx=zx;
                pomzy=zy;
                pomkx=kx;
                pomky=ky;
                pomdel=delka;
            }

        }
    }
    *x1 = pomzx; //vracim nejdelsi primku
    *x2 = pomkx;
    *y1 = pomzy;
    *y2 = pomky;
    return pomdel;
}

int find_hline(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2)
{
    int delka, sloupce, radky, zx, zy, kx, ky, pomzx, pomzy, pomkx, pomky, pomdel;
    pomdel = delka = zx = zy = kx = ky = pomzx = pomzy = pomkx = pomky = 0;
    for(radky = 0; radky < bitmap->rows; radky++)
    {
        zx = kx = radky;
        delka = 0;
        for(sloupce = 0; sloupce < bitmap->cols; sloupce++)
        {
            if(getcolor(bitmap,sloupce,radky)==1)
            {
                if(delka==0)
                {
                    zy = ky = sloupce;
                }
                else
                {
                    ky = sloupce;
                }
                delka++;
            }
            else
            {
                if(delka!=0)
                {
                    if(delka>pomdel)
                    {
                        pomzx=zx;
                        pomzy=zy;
                        pomkx=kx;
                        pomky=ky;
                        pomdel=delka;
                    }
                    delka=0;
                }
            }

            if(delka>pomdel)
            {
                pomzx=zx;
                pomzy=zy;
                pomkx=kx;
                pomky=ky;
                pomdel=delka;
            }
        }
    }
    *x1 = pomzx;
    *x2 = pomkx;
    *y1 = pomzy;
    *y2 = pomky;
    return pomdel;

}

Square getsquare(Bitmap *bitmap, int sloupce, int radky)
{
    Square vysledek; // ctverec pro vysledek
    vysledek.x1 = sloupce; // nastavim pocatecni a koncove souradnice na aktualni sloupec a radek
    vysledek.y1 = radky;
    vysledek.x2 = sloupce;
    vysledek.y2 = radky;
    int max;
    int i;
    int chyba = 0; // chyba nenalezena
    max = ((bitmap->rows-radky) > (bitmap->cols-sloupce)) ? bitmap->cols-sloupce : bitmap->rows-radky; // nastavim nejvetsi moznou velikost strany ctverce ktera se do pole jeste vejde
    for(i = 0; i <= max; i++)
    {
        if(getcolor(bitmap,sloupce,radky+i)==0) break;
        if(getcolor(bitmap,sloupce+i,radky)==0) break;
    }
    max = i;
    for(i = max-1; i >= 0; i--)   // pro max - 0 velikost ctverce prochazime pole
    {
        for(int novysloupec = 0; novysloupec <= i; novysloupec++)   // kontroluju kazdy novy radek jestli je ze samych 1 (jinak nastavuju chybu na 1 a vyskakuju z cyklu)
        {
            if(getcolor(bitmap, sloupce + novysloupec, radky+i) == 0)
            {
                chyba = 1;
                break;
            }

        }

        if(chyba == 1) break;
        for(int novyradek = 0; novyradek < i; novyradek++)   // kontroluju kazdy novy sloupec....
        {
            if(getcolor(bitmap, sloupce+i, radky+novyradek) == 0)
            {
                chyba = 1;
                break;
            }

        }
        if (chyba == 1) // pokud se stala chyba, kontroluju pro velikost strany o 1 mensi
        {
            chyba = 0;
            continue;
        }
        vysledek.x2 = sloupce + i; // pruchod probehl bez chyby, nastavuju koncove souradnice
        vysledek.y2 = radky + i;
        break; // pokud se chyba nestala, byl nalezen nejvetsi mozny ctverec
    }
    vysledek.plocha = (vysledek.x2 - vysledek.x1) * (vysledek.y2 - vysledek.y1); // vypocitam plochu
    vysledek.vzdalenost = (vysledek.x1 * vysledek.x1) + (vysledek.y1 * vysledek.y1); // vypocitam vzdalenost od 0,0
    return vysledek; // vracim nalezeny ctverec
}


int find_square(Bitmap *bitmap, int *x1, int *y1, int *x2, int *y2)
{
    int sloupce, radky;
    Square nejvetsi, docasny; // dve promenne typu Square
    nejvetsi.plocha = 0; // nulovani nejvetsiho ctverce (zatim jsem zadny nenasel)
    nejvetsi.x1 = 0;
    nejvetsi.x2 = 0;
    nejvetsi.y1 = 0;
    nejvetsi.y2 = 0;
    nejvetsi.vzdalenost = 0;
    for(sloupce = 0; sloupce < bitmap->cols; sloupce++)
    {
        for(radky = 0; radky < bitmap->rows; radky++)   // prochazim pole bunku po bunce
        {
            if(getcolor(bitmap, sloupce, radky) == 1)   // pokud jsem nasel 1
            {
                docasny = getsquare(bitmap, sloupce, radky); // najdu nejvetsi ctverec zacinajici v soucasnem bode
                if(docasny.plocha == nejvetsi.plocha)   // pokud ma nove nalezeny ctverec stejnou plochu jako nejvetsi nalezeny, zkontroluju jestli je blize souradnicim 0,0 nez nejvetsi nalezeny
                {
                    if(docasny.vzdalenost < nejvetsi.vzdalenost)
                    {
                        nejvetsi = docasny;
                    }
                }
                else if(docasny.plocha > nejvetsi.plocha) nejvetsi = docasny;   // nahradim nejvetsi nove nalezenym
            }
        }
    }
    *x1 = nejvetsi.y1; // vracim nalezene souradnice nejvetsiho
    *x2 = nejvetsi.y2;
    *y1 = nejvetsi.x1;
    *y2 = nejvetsi.x2;
    return nejvetsi.plocha;
}


int main(int argc, char * argv[])
{

    if(argc == 2) // help
    {
        if (strcmp(argv[1],"--help") == 0)
        {
            help();
            return 0;
        }
        else fprintf(stderr,"Zadali jste spatny argument programu, zadejte si --help\n");
    }

    else if (argc == 3) // test,hline, vline, nebo square + soubor
    {
        if (strcmp(argv[1],"--test")==0)
        {
            FILE* soubor;
            Bitmap main_bitmap;
            if ((soubor = fopen(argv[2],"r")) == NULL)
            {
                fprintf(stderr, "Chyba nacitani souboru\n");
                return 1;
            }
            else
            {
                fscanf(soubor,"%d",&main_bitmap.rows);
                fscanf(soubor,"%d",&main_bitmap.cols);
                alloc2D(&main_bitmap);
                if (readArray2D(&main_bitmap,soubor)==1)
                {
                    printf("invalid\n");
                }
                else
                {
                    printf("valid\n");
                }
            }

            free(main_bitmap.cells);
        }

        else if (strcmp(argv[1],"--hline")==0)
        {
            FILE* soubor;
            Bitmap main_bitmap;
            if ((soubor = fopen(argv[2],"r")) == NULL)
            {
                fprintf(stderr, "Chyba nacitani souboru\n");
                return 1;
            }
            else
            {
                fscanf(soubor,"%d",&main_bitmap.rows);
                fscanf(soubor,"%d",&main_bitmap.cols);
                alloc2D(&main_bitmap);
                readArray2D(&main_bitmap,soubor);
                if (readArray2D(&main_bitmap,soubor)==1)
                {
                    fprintf(stderr,"invalidni matice\n");
                }
                else
                {
                    int x1, x2, y1, y2;
                    find_hline(&main_bitmap, &x1, &y1, &x2, &y2);
                    if(x1==x2 && y1==y2)
                    {
                        fprintf(stderr,"nenalezen zadany utvar\n");
                    }
                    else
                    {
                        printf("%d %d %d %d\n", x1, y1, x2, y2);
                    }
                }
            }

            free(main_bitmap.cells);
        }

        else if (strcmp(argv[1],"--vline")==0)
        {
            FILE* soubor;
            Bitmap main_bitmap;
            if ((soubor = fopen(argv[2],"r")) == NULL)
            {
                fprintf(stderr, "Chyba nacitani souboru\n");
                return 1;
            }
            else
            {
                fscanf(soubor,"%d",&main_bitmap.rows);
                fscanf(soubor,"%d",&main_bitmap.cols);
                alloc2D(&main_bitmap);
                if (readArray2D(&main_bitmap,soubor)==1)
                {
                    fprintf(stderr,"invalidni matice\n");
                }
                else
                {
                    int x1, x2, y1, y2;
                    find_vline(&main_bitmap, &x1, &y1, &x2, &y2);
                    if(x1==x2 && y1==y2)
                    {
                        fprintf(stderr,"nenalezen zadany utvar\n");
                    }
                    else
                    {
                        printf("%d %d %d %d\n", x1, y1, x2, y2);
                    }
                }
            }

            free(main_bitmap.cells);
        }

        else if (strcmp(argv[1],"--square")==0)
        {

            FILE* soubor;
            Bitmap main_bitmap;
            if ((soubor = fopen(argv[2],"r")) == NULL)
            {
                fprintf(stderr, "Chyba nacitani souboru\n");
                return 1;
            }
            else
            {
                fscanf(soubor,"%d",&main_bitmap.rows);
                fscanf(soubor,"%d",&main_bitmap.cols);
                alloc2D(&main_bitmap);
                if (readArray2D(&main_bitmap,soubor)==1)
                {
                    fprintf(stderr,"invalidni matice\n");
                }
                else
                {
                    int x1, x2, y1, y2;
                    find_square(&main_bitmap, &x1, &y1, &x2, &y2);
                    if(x1==0 && x2==0 && y1==0 && y2==0)
                    {
                        fprintf(stderr,"nenalezen zadany utvar\n");
                    }
                    else
                    {
                        printf("%d %d %d %d\n", x1, y1, x2, y2);
                    }
                }
            }

            free(main_bitmap.cells);
        }

        else fprintf(stderr,"Zadali jste spatny argument programu, zadejte si --help\n");

    }

}
