[bits 16]
print_string:
  pusha
  mov ah, 0x0e

loop:
  mov al, [bx]
  cmp al, 0
  je end
  int 0x10
  add bx, 0x1
  jmp loop

end:
  popa
  ret