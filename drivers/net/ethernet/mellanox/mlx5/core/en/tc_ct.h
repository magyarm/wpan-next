/* SPDX-License-Identifier: GPL-2.0 OR Linux-OpenIB */
/* Copyright (c) 2018 Mellanox Technologies. */

#ifndef __MLX5_EN_TC_CT_H__
#define __MLX5_EN_TC_CT_H__

#include <net/pkt_cls.h>
#include <linux/mlx5/fs.h>
#include <net/tc_act/tc_ct.h>

struct mlx5_esw_flow_attr;
struct mlx5e_tc_mod_hdr_acts;
struct mlx5_rep_uplink_priv;
struct mlx5e_tc_flow;
struct mlx5e_priv;

struct mlx5_ct_flow;

struct nf_flowtable;

struct mlx5_ct_attr {
	u16 zone;
	u16 ct_action;
	struct mlx5_ct_flow *ct_flow;
	struct nf_flowtable *nf_ft;
};

#define zone_to_reg_ct {\
	.mfield = MLX5_ACTION_IN_FIELD_METADATA_REG_C_2,\
	.moffset = 0,\
	.mlen = 2,\
	.soffset = MLX5_BYTE_OFF(fte_match_param,\
				 misc_parameters_2.metadata_reg_c_2) + 2,\
}

#define ctstate_to_reg_ct {\
	.mfield = MLX5_ACTION_IN_FIELD_METADATA_REG_C_2,\
	.moffset = 2,\
	.mlen = 2,\
	.soffset = MLX5_BYTE_OFF(fte_match_param,\
				 misc_parameters_2.metadata_reg_c_2),\
}

#define mark_to_reg_ct {\
	.mfield = MLX5_ACTION_IN_FIELD_METADATA_REG_C_3,\
	.moffset = 0,\
	.mlen = 4,\
	.soffset = MLX5_BYTE_OFF(fte_match_param,\
				 misc_parameters_2.metadata_reg_c_3),\
}

#define labels_to_reg_ct {\
	.mfield = MLX5_ACTION_IN_FIELD_METADATA_REG_C_4,\
	.moffset = 0,\
	.mlen = 4,\
	.soffset = MLX5_BYTE_OFF(fte_match_param,\
				 misc_parameters_2.metadata_reg_c_4),\
}

#define fteid_to_reg_ct {\
	.mfield = MLX5_ACTION_IN_FIELD_METADATA_REG_C_5,\
	.moffset = 0,\
	.mlen = 4,\
	.soffset = MLX5_BYTE_OFF(fte_match_param,\
				 misc_parameters_2.metadata_reg_c_5),\
}

#define tupleid_to_reg_ct {\
	.mfield = MLX5_ACTION_IN_FIELD_METADATA_REG_C_1,\
	.moffset = 0,\
	.mlen = 3,\
	.soffset = MLX5_BYTE_OFF(fte_match_param,\
				 misc_parameters_2.metadata_reg_c_1),\
}

#define TUPLE_ID_BITS (mlx5e_tc_attr_to_reg_mappings[TUPLEID_TO_REG].mlen * 8)
#define TUPLE_ID_MAX GENMASK(TUPLE_ID_BITS - 1, 0)

#if IS_ENABLED(CONFIG_MLX5_TC_CT)

int
mlx5_tc_ct_init(struct mlx5_rep_uplink_priv *uplink_priv);
void
mlx5_tc_ct_clean(struct mlx5_rep_uplink_priv *uplink_priv);

int
mlx5_tc_ct_parse_match(struct mlx5e_priv *priv,
		       struct mlx5_flow_spec *spec,
		       struct flow_cls_offload *f,
		       struct netlink_ext_ack *extack);
int
mlx5_tc_ct_parse_action(struct mlx5e_priv *priv,
			struct mlx5_esw_flow_attr *attr,
			const struct flow_action_entry *act,
			struct netlink_ext_ack *extack);

struct mlx5_flow_handle *
mlx5_tc_ct_flow_offload(struct mlx5e_priv *priv,
			struct mlx5e_tc_flow *flow,
			struct mlx5_flow_spec *spec,
			struct mlx5_esw_flow_attr *attr,
			struct mlx5e_tc_mod_hdr_acts *mod_hdr_acts);
void
mlx5_tc_ct_delete_flow(struct mlx5e_priv *priv,
		       struct mlx5e_tc_flow *flow,
		       struct mlx5_esw_flow_attr *attr);

bool
mlx5e_tc_ct_restore_flow(struct mlx5_rep_uplink_priv *uplink_priv,
			 struct sk_buff *skb, u32 tupleid);

#else /* CONFIG_MLX5_TC_CT */

static inline int
mlx5_tc_ct_init(struct mlx5_rep_uplink_priv *uplink_priv)
{
	return 0;
}

static inline void
mlx5_tc_ct_clean(struct mlx5_rep_uplink_priv *uplink_priv)
{
}

static inline int
mlx5_tc_ct_parse_match(struct mlx5e_priv *priv,
		       struct mlx5_flow_spec *spec,
		       struct flow_cls_offload *f,
		       struct netlink_ext_ack *extack)
{
	return -EOPNOTSUPP;
}

static inline int
mlx5_tc_ct_parse_action(struct mlx5e_priv *priv,
			struct mlx5_esw_flow_attr *attr,
			const struct flow_action_entry *act,
			struct netlink_ext_ack *extack)
{
	return -EOPNOTSUPP;
}

static inline struct mlx5_flow_handle *
mlx5_tc_ct_flow_offload(struct mlx5e_priv *priv,
			struct mlx5e_tc_flow *flow,
			struct mlx5_flow_spec *spec,
			struct mlx5_esw_flow_attr *attr,
			struct mlx5e_tc_mod_hdr_acts *mod_hdr_acts)
{
	return ERR_PTR(-EOPNOTSUPP);
}

static inline void
mlx5_tc_ct_delete_flow(struct mlx5e_priv *priv,
		       struct mlx5e_tc_flow *flow,
		       struct mlx5_esw_flow_attr *attr)
{
}

static inline bool
mlx5e_tc_ct_restore_flow(struct mlx5_rep_uplink_priv *uplink_priv,
			 struct sk_buff *skb, u32 tupleid)
{
	if  (!tupleid)
		return  true;

	return false;
}

#endif /* !IS_ENABLED(CONFIG_MLX5_TC_CT) */
#endif /* __MLX5_EN_TC_CT_H__ */
