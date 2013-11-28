include host.mk
TOP = pif
NAME= pif
FILES = pif.vhd
DEV= LCMXO2-7000HC-4TG144C
DDIR = /usr/local/diamond/2.2_x64/bin/lin64/

all: init
	$(DDIR)diamondc syn.tcl
	@cp build/$(NAME)_build.jed ./$(NAME).jed

init: clean
	@echo "prj_project new -name $(NAME) -impl build -dev $(DEV) -lpf ../layout/$(NAME).lpf" > new_prj.tcl
	$(foreach FILE, $(FILES), echo "prj_src add ../rtl/$(FILE)" >> new_prj.tcl;)
	@echo "prj_project save" >> new_prj.tcl
	@echo "prj_project close" >> new_prj.tcl
	$(DDIR)diamondc new_prj.tcl

	@echo "prj_project open $(NAME).ldf" > syn.tcl
	@echo "prj_run Synthesis -impl build" >> syn.tcl
	@echo "prj_run Translate -impl build" >> syn.tcl
	@echo "prj_run Map -impl build" >> syn.tcl
	@echo "prj_run PAR -impl build" >> syn.tcl
	@echo "prj_run PAR -impl build -task PARTrace" >> syn.tcl
	@echo "prj_run Export -impl build -task Bitgen" >> syn.tcl
	@echo "prj_project close" >> syn.tcl

deploy:
	scp $(NAME).jed $(HOST):/tmp/build.jed
	ssh $(HOST) $(PIFLOAD) /tmp/build.jed
	ssh $(HOST) $(PIFLOAD) rm /tmp/build.jed

clean:
	rm -rf build
	rm -rf $(NAME)*
	rm -rf *.tcl

.PHONY: clean

