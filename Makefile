all:
	@echo "           boing         boing         boing"   
	@echo " e-e           . - .         . - .         . - ."  
	@echo "(\_/)\       '       \`.   ,'       \`.   ,'       ."  
	@echo " \`-'\ \`--.___,         . .           . .          ."  
	@echo "    '\( ,_.-'                                        "  
	@echo "       \\               \"             \"          Compile !"    
	@echo "       ^\'"
	$(MAKE) -C src

.PHONY: clean distclean build-dep help

clean:
	$(MAKE) -C src clean

distclean:
	$(MAKE) -C src distclean

build-deps:
	$(MAKE) -C src build-deps

help:
	@echo "There are four extra targets besides the default 'all':"
	@echo
	@echo "clean      - Deletes all temporary object files after compilation."
	@echo "distclean  - Deletes all temporary object files and the compiled executable."
	@echo "build-deps - Create 'make.dep' in 'src/' directory."
	@echo "help       - Shows this message."
