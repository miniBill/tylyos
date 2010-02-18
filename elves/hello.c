int main(){
  char test[]={'T','e','s','t','\n',0};
  asm("int $0x80"::"a"(250),"b"(test));
  return 0;
}
