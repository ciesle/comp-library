SHELL := bash
.SHELLFLAGS := -o pipefail -ec
CC = g++
CFLAGS_OPT = -std=gnu++23 -O2 -I../library/include -fsanitize=address,undefined
CFLAGS_DBG = -std=gnu++23 -O0 -g3 -I../library/include -fsanitize=address,undefined
OUT_DIR = ./.vscode/auto_build

LIBRARIES:=$(shell find ./snippet/ -type f)
SNIP_PATH=./.vscode/comp.json.code-snippets
ADD_SH=../library/add.sh

$(OUT_DIR)/main: main.cpp
	$(CC) $(CFLAGS_OPT) -o $@ $^
$(OUT_DIR)/brute: brute.cpp
	$(CC) $(CFLAGS_OPT) -o $@ $^
$(OUT_DIR)/gen: gen.cpp
	$(CC) $(CFLAGS_OPT) -o $@ $^
$(OUT_DIR)/main_dbg: main.cpp
	$(CC) $(CFLAGS_DBG) -o $@ $^ 

main: $(OUT_DIR)/main
	@rm -f ./$@
	@cp $^ ./
brute: $(OUT_DIR)/brute
	@rm -f ./$@
	@cp $^ ./
gen: $(OUT_DIR)/gen
	@rm -f ./$@
	@cp $^ ./
main_dbg: $(OUT_DIR)/main_dbg
	@rm -f ./$@
	@cp $^ ./
diff: diff.cpp
	$(CC) $(CFLAGS_OPT) -o $@ $^

.PHONY: clean
clean:
	@rm -f main brute gen main_dbg a.out test rnd* $(OUT_DIR)/*

.PHONY: clean_main
clean_main:
	@rm -f main $(OUT_DIR)/main*

.PHONY: gdb_clean
gdb_clean:
	@ls peda-session* && rm peda-session* || :
	@if [ -e .gdb_history ]; then rm gdb_history; fi

.PHONY: copy
copy:
	@printf "\e[35;1mcopy start\e[m\n"
	@include-what-you-use -std=gnu++23 -I../library/include main.cpp 2>&1 | ../library/expand_include.py | xsel -bi
	@printf "\e[35;1mcopy end\e[m\n"

.PHONY: snippet
snippet:
	@printf "{\n\t\n}" > $(SNIP_PATH)
	@$(foreach file,$(LIBRARIES),$(ADD_SH) $(file);)
	@sed -i -e :a -e '$$d;N;2,2ba' -e 'P;D' $(SNIP_PATH)
	@echo "\t}\n}" >> $(SNIP_PATH)