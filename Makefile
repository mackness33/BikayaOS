
default:
	@echo "Please specify a target"

umps:
	$(MAKE) -f umpsmake0

umps1:
	$(MAKE) -f phase1.0/umpsmake0

uarm:
	$(MAKE) -f uarmmake0

uarm1:
	$(MAKE) -f phase1.0/uarmmake0


clean:
	$(MAKE) -f uarmmake0 clean
	$(MAKE) -f umpsmake0 clean

clean1:
	$(MAKE) -f phase1.0/uarmmake0 clean
	$(MAKE) -f phase1.0/umpsmake0 clean

.PHONY: umps umps2 uarm clean default
