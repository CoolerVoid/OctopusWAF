all:
	@echo "           boing         boing         boing"   
	@echo " e-e           . - .         . - .         . - ."  
	@echo "(\_/)\       '       \`.   ,'       \`.   ,'       ."  
	@echo " \`-'\ \`--.___,         . .           . .          ."  
	@echo "    '\( ,_.-'                                        "  
	@echo "       \\               \"             \"          Compile !"    
	@echo "       ^\'"
	$(MAKE) -C src

.PHONY: clean distclean

clean:
	$(MAKE) -C src clean

distclean:
	$(MAKE) -C src distclean


