# 智能配餐推荐系统 - 部署指南

本指南将指导您如何将智能配餐推荐系统部署为容器化应用，使其可以在任何支持 Docker 的环境中运行。

## 前置要求

- [Docker](https://docs.docker.com/get-docker/)
- [Docker Compose](https://docs.docker.com/compose/install/)

## 快速开始

### 1. 使用 Docker Compose (推荐)

我们提供了 `docker-compose.yml` 文件，可以一键构建并运行系统。

**构建镜像：**

```bash
docker-compose build
```

**运行系统：**

你可以直接使用我们提供的脚本：

```bash
./run_docker.sh
```

或者手动运行：

由于本系统是一个交互式命令行程序，需要使用 `run` 命令而不是 `up`，并且不需要 `-d` (后台运行)。

```bash
docker-compose run --rm meal-system
```

- `--rm`：容器退出后自动删除，保持环境清洁。
- 数据持久化：系统数据会自动保存在 `meal_data` 卷中，即使删除容器，数据也不会丢失。

### 2. 使用 Docker 手动构建和运行

如果您不想使用 Docker Compose，也可以手动使用 Docker 命令。

**构建镜像：**

```bash
docker build -t meal-recommendation-system .
```

**运行容器：**

```bash
docker run -it --rm \
  -v meal_data_manual:/app/build/bin/data \
  meal-recommendation-system
```

- `-it`：开启交互式终端，这对本系统是必须的。
- `-v ...`：挂载数据卷，确保用户数据和历史记录持久化保存。

## 数据备份与迁移

如果您使用 Docker Compose 部署，数据存储在名为 `meal_data` 的 Docker 卷中。

**查看数据卷位置：**

```bash
docker volume inspect meal_data
```

**备份数据：**

您可以将数据卷中的文件复制出来进行备份：

```bash
# 创建一个临时容器来访问数据卷并打包
docker run --rm -v meal_data:/data -v $(pwd):/backup ubuntu tar cvf /backup/meal_data_backup.tar /data
```

**恢复数据：**

```bash
# 将备份数据解压回数据卷
docker run --rm -v meal_data:/data -v $(pwd):/backup ubuntu bash -c "cd / && tar xvf /backup/meal_data_backup.tar"
```

## 常见问题

### 1. 中文显示乱码

我们在 Docker 镜像中已经配置了完整的 UTF-8 中文环境 (`zh_CN.UTF-8`)，应该可以正常显示中文。如果仍然遇到乱码，请检查您的终端模拟器（如 iTerm2, Windows Terminal, PuTTY 等）的编码设置是否为 UTF-8。

### 2. 无法输入

请确保在运行 `docker run` 时加上了 `-it` 参数，或在 `docker-compose.yml` 中配置了 `stdin_open: true` 和 `tty: true`（我们已经默认配置好了）。

### 3. 数据丢失

请确保运行时使用了 Volume 挂载（Docker Compose 默认已配置）。如果您直接使用 `docker run` 而没有 `-v` 参数，容器重启后数据将会重置。
