```
Torrent 文件（扩展名为 .torrent，常被称为“种子”）本质上是一个不包含实际文件内容，而是包含目标文件元数据的“索引文件”。
结合你正在开发的 C++ 解析器，以下是 Torrent 文件的底层格式规范与结构解析：
1. 编码格式：Bencode
Torrent 文件并非普通的文本文件，它采用一种名为 Bencode 的二进制编码格式进行存储。Bencode 仅支持四种基本数据类型：
字符串 (String)：格式为 <长度>:<内容>，例如 4:spam。
整数 (Integer)：使用 i 和 e 包裹，例如 i123e。
列表 (List)：使用 l 和 e 包裹，例如 l4:spam4:eggse。
字典 (Dictionary)：使用 d 和 e 包裹，键值对交替出现，且键（Key）必须是字符串并按字母顺序排列，例如 d4:spam4:eggse。
2. 核心数据结构
一个典型的 .torrent 文件在 Bencode 解码后，整体呈现为一个字典结构，主要包含以下核心字段：
announce (字符串，必需)：Tracker 服务器的 URL 地址，用于协调 P2P 节点之间的连接。
announce-list (列表，可选)：包含多个备用 Tracker 服务器的 URL 列表，用于冗余和负载均衡。
creation date (整数，可选)：种子文件创建的时间（Unix 时间戳）。
created by (字符串，可选)：生成该种子的软件名称（如 uTorrent、qBittorrent）。
comment (字符串，可选)：用户或发布者添加的备注信息。
info (字典，必需)：这是整个种子文件中最核心的部分，包含了文件的详细信息，决定了文件的完整性验证机制。
3. info 字典的内部结构
info 字段根据下载的是单文件还是多文件，其内部结构会有所不同：
单文件模式：
name：建议保存的文件名称。
length：文件的总大小（以字节为单位）。
piece length：每个分块的大小（必须是 2K 的整数次方）。
pieces：每个文件块的 SHA-1 哈希值拼接而成的字节串，用于校验下载数据的完整性。
多文件模式：
name：建议保存的顶层目录名称。
piece length 与 pieces：同上，用于分块和校验。
files (列表)：包含多个字典的列表，每个字典代表一个文件，包含：
length：该文件的大小。
path (列表)：文件的相对路径，最后一项为实际文件名。
4. 关键机制：Info Hash
客户端向 Tracker 服务器发起请求时，并不是使用整个 .torrent 文件，而是对 info 字典的 Bencode 编码数据进行 SHA-1 哈希运算，生成一个 20 字节的特征码（即 info_hash）。只要两个种子的 info 部分完全相同，它们的 info_hash 就一致，其余字段（如 comment 或 announce）的修改不会影响原文件的下载。
```