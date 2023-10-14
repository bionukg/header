#pragma once

#ifndef bionukg_optimize
#define bionukg_optimize
#define buffer_capacity 256

#define memcpy_biopt(_dest,_sour,size) {for(int i=0;i<size/4;i++){((unsigned*)_dest)[i]=((unsigned*)_sour)[i];};for(int i=((size>>2)<<2);i<size;i++){((char*)_dest)[i]=((char*)_sour)[i];};}
#define fputs_biopt(_char,_fp,buffer,idx) {buffer[idx]=_char;if(idx==buffer_capacity-1){fwrite(buffer,1,buffer_capacity,_fp);idx=0;}else{idx++; }}
#define fwrite_biopt(_wbuffer,_size,_fp,buffer,idx) {if(idx+size<buffer_capacity){memcpy_biopt((char*)buffer+idx,_wbuffer,_size);idx+=size;}else { fendput_biopt(_fp, buffer, idx); fwrite(_wbuffer, 1, _size, _fp); }}
#define fendput_biopt(_fp,buffer,idx) {fwrite(buffer, 1, idx, _fp),idx=0;}



#endif