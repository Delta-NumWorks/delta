executables += test.external_flash.write test.external_flash.read

$(BUILD_DIR)/test.external_flash.%.$(EXE): LDSCRIPT = ion/test/device/n0100/external_flash_tests.ld
test_external_flash_src = $(ion_src) $(liba_src) $(libaxx_src) $(kandinsky_src) $(poincare_src) $(ion_device_dfu_relogated_src) $(runner_src)
$(BUILD_DIR)/test.external_flash.read.$(EXE): $(BUILD_DIR)/quiz/src/test_ion_external_flash_read_symbols.o $(call object_for,$(test_external_flash_src) $(test_ion_external_flash_read_src))
$(BUILD_DIR)/test.external_flash.write.$(EXE): $(BUILD_DIR)/quiz/src/test_ion_external_flash_write_symbols.o $(call object_for,$(test_external_flash_src) $(test_ion_external_flash_write_src))

.PHONY: %_flash
%_flash: $(BUILD_DIR)/%.dfu $(BUILD_DIR)/flasher.light.dfu
	@echo "DFU     $@"
	@echo "INFO    About to flash your device. Please plug your device to your computer"
	@echo "        using an USB cable and press the RESET button on the back of your"
	@echo "        device."
	$(Q) until $(PYTHON) build/device/dfu.py -l | grep -E "0483:a291|0483:df11" > /dev/null 2>&1; do sleep 2;done
	$(Q) $(PYTHON) build/device/dfu.py -u $(word 2,$^)
	$(Q) sleep 2
	$(Q) $(PYTHON) build/device/dfu.py -u $(word 1,$^)
	$(Q) sleep 2
	$(Q) $(PYTHON) build/device/dfu.py --exit
