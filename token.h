/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kmaeda <kmaeda@student.42berlin.de>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/01 14:06:32 by kmaeda            #+#    #+#             */
/*   Updated: 2025/08/01 14:16:30 by kmaeda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TOKEN.H
#define TOKEN.H

t_list	*tokenize(const char *input);
void	free_tokens(t_list *token_list);
char	**list_to_array(t_list *tokens);

#endif
