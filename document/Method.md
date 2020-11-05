# Method

### 生成草场
```mermaid
graph LR
	VS -->|提供地面元数据| TS
	TS -->|细分地面区域| GS
	GS -->|在每个细分的上面种草| FS
```

### 