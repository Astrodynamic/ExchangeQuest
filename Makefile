# Пути и переменные
BUILD := ./build
CMAKE := cmake

# Основные цели
.PHONY: all
all: release

.PHONY: release debug
release debug:
	@$(CMAKE) --preset $@
	@$(CMAKE) --build $(BUILD) --target all -j $(shell nproc)

.PHONY: test
test: debug
	$(BUILD)/server/tests/TEST

# Удаление сгенерированных файлов
.PHONY: clean
clean:
	@find ./ -name "build" -type d -exec rm -rf {} +
	@find . -name "*.user" -type f -delete