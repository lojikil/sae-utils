/* xteaofb: XTEA in Ouput Feed Back mode, of sorts.
 * Basically, a nice little stream cipher out of
 * XTEA. 
 * The author of this program releases all claims of
 * copyright & places this in the public domain. In those
 * countries where the notion of 'public domain' doesn't
 * exist, it is copyright 2011 Stefan Edwards, under the zlib/png
 * license. Please see the LICENSE file for details.
 *
 * NB:
 *   0 - Although this program uses a standard algorithm,
 *   no claim is made by its author that mistakes in 
 *   implementation have been avoided. 
 *   1 - IANAC (I Am Not A Cryptographer), although I play
 *   one on IRC. Basically, if you have some super-secret
 *   data, I'd rely on a standard product. This is just a
 *   test program, to play with OFB & XTea.
 */

#include <stdio.h>
#include <stdint.h>

#define nil NULL
#define nul '\0'
#define num_rounds 64

#define massert(x,y) if((x)) printf("%%DEBUG-%s \"%s\"\n",__FUNCTION__,y)

void encipher(uint32_t* v, uint32_t* k)
{
    uint32_t v0=v[0], v1=v[1], i;
    uint32_t sum=0, delta=0x9E3779B9;
    for(i=0; i < num_rounds; i++)
    {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + k[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + k[(sum>>11) & 3]);
    }
    v[0]=v0;
    v[1]=v1;
}

uint32_t *keymaker(char *s)
{
        uint32_t *rc = nil, *tmph = nil;
        char *keydat = nil; /* to be buffered */
        int len = strlen(s), tmp = strlen(s);
        if(tmp > 16)
                tmp = 16;
        else if(tmp < 16)
                tmp += 16 - tmp;
        if((keydat = malloc(sizeof(char) * tmp))  == nil)
                return nil;
        strcpy(keydat,s);
        for(;len < 16;len++)
                keydat[len] = nul;
        if((rc = malloc(sizeof(char) * 4)) != nil)
        {
                tmph = (uint32_t *)keydat;
                rc[0] = tmph[0];
                rc[1] = tmph[1];
                rc[2] = tmph[2];
                rc[3] = tmph[3];
        }
        free(keydat);
        return rc;
}

int
main(int ac, char **al)
{
        const uint32_t IV[2] = {0xcafebabe,0xdeadbeef}; 
        /* Hashes of the above:
         * MD5: 0ddcc4a96d4372f29529ab01e58ad6ab
         * SHA1: 2c9a9788ec1e223b17148f38b002a668891336de
         * Currently, the IV is constant for every run of the system; this
         * is a poor design, since the IV plays so strongly in OFB. Once a
         * 'live' version of this program is released, the IVs need to be
         * able to be modified... Keys & IVs should be zero-padded.
         */
        FILE *fdin = nil, *fdout = nil;
        char inbuf[8] = {0};
        uint32_t *outdat = nil, *key = nil;
        uint32_t *res = nil, *indat = nil;
        if(ac != 4)
        {
                printf("Usage: xteaofb input output key\nUse '-' for stdin/out\n");
                return 1;
        }
        if(!strcmp(al[1],"-"))
                fdin = stdin;
        else
                if((fdin = fopen(al[1],"r")) == nil)
                {
                        printf("cannot open \"%s\" for reading\n",al[1]);
                        return 2;
                }
        if(!strcmp(al[2],"-"))
                fdout = stdout;
        else
                if((fdout = fopen(al[2],"w")) == nil)
                {
                        printf("cannot open \"%s\" for writing\n",al[2]);
                        fclose(fdin);
                        return 3;
                }
        if((outdat = malloc(sizeof(uint32_t) * 2)) == nil)
        {
                printf("Cannot malloc outdat\n");
                fclose(fdin);
                fclose(fdout);
        }
        if((res = malloc(sizeof(uint32_t) * 2)) == nil)
        {
                printf("cannot malloc res\n");
                free(outdat);
                fclose(fdin);
                fclose(fdout);
        }
        /* need to extract keys...*/
        key = keymaker(al[3]);  
        massert(key == nil,"key == nil");        
        /* visual sanity check of keymaker */
        //printf("%%DEBUG-KEYMAKER:%x,%x,%x,%x\n",key[0],key[1],key[2],key[3]); 
        res[0] = IV[0];
        res[1] = IV[1];        
        /* pack indat by cheating */    
        indat = (uint32_t *)&inbuf;        
        while(1)
        {
                encipher(res,key);        
                //printf("%%DEBUG-main-res: %x,%x\n",res[0],res[1]);
                if(feof(fdin))
                	break; 
                fread(inbuf,sizeof(char),8,fdin);
                outdat[0] = indat[0] ^ res[0];
                outdat[1] = indat[1] ^ res[1];
                fwrite(outdat,sizeof(uint32_t),2,fdout);
        }
        free(res);
        free(key);
        free(outdat);        
        fclose(fdin);
        fclose(fdout);
        return 0;
}
