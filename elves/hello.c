int main(){
  char test[]={'T','e','s','t','\n',0};
  asm("movw $0x250,%%ax\n\t"
      "int $0x80"::"b"(test));
  return 0;
}
