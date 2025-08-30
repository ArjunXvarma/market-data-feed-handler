# market-data-feed-handler

```mermaid
flowchart LR

subgraph NetworkLayer["Network Layer"]
A[UDP Multicast Subscriber] --> B[Packet Buffer]
end

subgraph ProcessingLayer["Processing Layer"]
B --> C[Parser Thread(s)]
C --> D[Message Dispatcher]
end

subgraph ApplicationLayer["Application Layer"]
D --> E[Consumer Interface]
D --> F[Statistics Collector]
end

subgraph OutputLayer["Output Layer"]
E --> G[Downstream Consumers (Strategies, Order Book, Logger)]
F --> H[Real-time Metrics Dashboard]
end
```