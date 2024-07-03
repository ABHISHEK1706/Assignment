#include <linux/bpf.h>
#include <linux/pkt_cls.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/tcp.h>

BPF_MAP_DEF(port_map) = {
    .map_type = BPF_MAP_TYPE_ARRAY,
    .key_size = sizeof(__u32),
    .value_size = sizeof(__u16),
    .max_entries = 1,
};
BPF_MAP_ADD(port_map);

SEC("classifier")
int drop_tcp_packets(struct __sk_buff *skb) {
    struct ethhdr *eth = bpf_hdr_pointer(skb, 0, sizeof(struct ethhdr));
    if (!eth)
        return TC_ACT_OK;

    if (eth->h_proto != bpf_htons(ETH_P_IP))
        return TC_ACT_OK;

    struct iphdr *ip = bpf_hdr_pointer(skb, sizeof(struct ethhdr), sizeof(struct iphdr));
    if (!ip || ip->protocol != IPPROTO_TCP)
        return TC_ACT_OK;

    struct tcphdr *tcp = bpf_hdr_pointer(skb, sizeof(struct ethhdr) + sizeof(struct iphdr), sizeof(struct tcphdr));
    if (!tcp)
        return TC_ACT_OK;

    __u32 key = 0;
    __u16 *port = bpf_map_lookup_elem(&port_map, &key);
    if (!port)
        return TC_ACT_OK;

    if (tcp->dest == bpf_htons(*port))
        return TC_ACT_SHOT;

    return TC_ACT_OK;
}

char _license[] SEC("license") = "GPL";
