/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: retanaka <retanaka@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 18:15:53 by retanaka          #+#    #+#             */
/*   Updated: 2024/05/09 14:20:56y retanaka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	*ft_memmove(char *dst, const char *src, size_t n)
{
	int	i;
	int	dif;
	int	end;

	if (src > dst)
	{
		i = 0;
		dif = 1;
		end = n;
	}
	else
	{
		i = n;
		dif = -1;
		end = -1;
	}
	while (i != end)
	{
		dst[i] = src[i];
		i += dif;
	}
	return (dst);
}

size_t	ft_strlen(char *s)
{
	size_t	i;
	size_t	j;

	if (s == NULL)
		return (0);
	i = 0;
	j = 0;
	while (s[i] != 0)
	{
		if (i < j)
			return ((size_t)(-1));
		j = i;
		i++;
	}
	return (i);
}

char	*create_str(char *s)
{
	char	*result;
	size_t	s_len;

	s_len = ft_strlen(s);
	result = (char *)malloc((s_len + 1) * sizeof(char));
	if (result == NULL)
		return (NULL);
	if (s != NULL)
		ft_memmove(result, s, s_len);
	result[s_len] = '\0';
	return (result);
}

void	concat_chr(char **s1, char c)
{
	char	*result;
	size_t	s1_len;

	s1_len = ft_strlen(*s1);
	if ((((size_t)(-1) - s1_len) <= 1) || (c == '\0'))
		return ;
	if (c == '\0')
		return ;
	result = (char *)malloc((s1_len + 2) * sizeof(char));
	if (result == NULL)
		return ;
	if (*s1 != NULL)
	{
		ft_memmove(result, *s1, s1_len);
		free(*s1);
	}
	result[s1_len] = c;
	result[s1_len + 1] = '\0';
	*s1 = result;
}

void	concat_str(char **s1, char *s2)
{
	char	*result;
	size_t	s1_len;
	size_t	s2_len;
	size_t	result_len;

	s1_len = ft_strlen(*s1);
	s2_len = ft_strlen(s2);
	result_len = s1_len + s2_len;
	if (((size_t)(-1) - s1_len <= s2_len) || (result_len == 0))
		return ;
	result = (char *)malloc((result_len + 1) * sizeof(char));
	if (result == NULL)
		return ;
	if (*s1 != NULL)
	{
		ft_memmove(result, *s1, s1_len);
		free(*s1);
	}
	if (s2 != NULL)
	{
		ft_memmove(result + s1_len, s2, s1_len);
		free(s2);
	}
	result[result_len] = '\0';
	*s1 = result;
}

char	convert_x(char c, int flag)
{
	if (0 <= c && c <= 9)
		return (c + '0');
	if (10 <= c && c <= 15)
	{
		if (flag == 0)
			return (c + 'a' - 10);
		else
			return (c + 'A' - 10);
	}
	return (0);
}

// char	*convert_pointer(void *p)
// {
// 	size_t	i;
// 	size_t	p_len;
// 	char	*result;
// 
// 	p_len = sizeof(p);
// 	result = (char *)malloc((p_len + 3) * sizeof(char));
// 	if (result == NULL)
// 		return (NULL);
// 	result[0] = '0';
// 	result[1] = 'x';
// 	i = 0;
// 	while (i < p_len)
// 	{
// 		result[i + 2] = convert_x(((unsigned long long)p >> (p_len - i)) & 0xf, 0);
// 		i++;
// 	}
// 	result[i + 2] = '\0';
// 	return (result);
// }

char	get_chr_base(int num, char base_chr)
{
	if (num < 10)
		return (num + '0');
	if (base_chr == 'X')
		return (num - 10 + 'A');
	else
		return (num - 10 + 'a');
}

int	ft_pow(int n1, int n2)
{
	int	result;

	if (n2 < 0)
		return (0);
	if (n1 == 0 || n2 == 0)
		return (1);
	result = n1;
	while (--n2 > 0)
		result *= n1;
	return (result);
}

int	get_dgt(int num, int base)
{
	int	i;

	i = 0;
	while (num >= base)
	{
		num /= base;
		i++;
	}
	return (i);
}

char	convert_num_chr(unsigned int *num, int base, int case_flag)
{
	int	dgt;
	int	dgt_value;
	int	print_value;

	dgt = get_dgt(*num, base);
	dgt_value = ft_pow(base, dgt);
	print_value = *num / dgt_value;
	*num -= print_value * dgt_value;
	return (get_chr_base(print_value, case_flag));
}

void	convert_base_num(char base_chr, int *base)
{
	if (base_chr == 'o')
		*base = 8;
	else if (base_chr == 'd' || base_chr == 'i' || base_chr == 'u')
		*base = 10;
	else if (base_chr == 'x' || base_chr == 'X')
		*base = 16;
}

char	*convert_num_str(int num, char base_chr)
{
	char			*result;
	int				base;
	unsigned int	u_num;
	int				u_num_len;
	char			*num_str;

	convert_base_num(base_chr, &base);
	result = NULL;
	if ((base_chr == 'd' || base_chr == 'i') && num < 0)
	{
		if ((long)num == -2147483648)
			return (create_str("-2147483648"));
		result = create_str("-");
		num = -num;
	}
	u_num = num;
	u_num_len = get_dgt(u_num, base);
	num_str = (char *)malloc(u_num_len + 1);
	num_str[u_num_len] = '\0';
	while (u_num_len--)
	{
		num_str[u_num_len] = get_chr_base(u_num % base, base_chr);
		u_num /= base;
	}
	concat_str(&result, num_str);
	return (result);
}

char	*convert_pointer(void *p)
{
	char	*result;

	result = create_str("0x");
	concat_str(&result, convert_num_str((int)p, 'x'));
	return (result);
}

void	convert_variable(char **str, const char c, va_list args)
{
	if (c == '%')
		concat_chr(str, '%');
	else if (c == 'c')
		concat_chr(str, (char)va_arg(args, int));
	else if (c == 's')
		concat_str(str, create_str(va_arg(args, char *)));
	else if (c == 'p')
		concat_str(str, convert_pointer(va_arg(args, void *)));
	else if (c == 'd' || c == 'i' || c == 'u'
		|| c == 'o' || c == 'x' || c == 'X')
		concat_str(str, convert_num_str(va_arg(args, int), c));
}

char	*generate_result(const char* source, va_list args)
{
	int	i;
	char	*result;

	if (source == NULL)
		return (NULL);
	result = NULL;
	i = 0;
	while (source[i] != '\0')
	{
		if (source[i] == '%')
		{
			i++;
			convert_variable(&result, source[i], args);
		}
		else
			concat_chr(&result, source[i]);
		i++;
	}
	return (result);
}

size_t	ft_putstr(char *s)
{
	size_t	s_len;

	if (s == NULL || ft_strlen(s) == 0)
		write(1, "error\n", 6);
	s_len = ft_strlen(s);
	write(1, s, s_len);
	return (s_len);
}

int	ft_printf(const char* source, ...)
{
	va_list	args;
	char	*result;
	size_t	result_len;

	va_start(args, source);
	result = generate_result(source, args);
	result_len = ft_putstr(result);
	free(result);
	va_end(args);
	return (result_len);
}

#ifdef TEST
#include<stdio.h>
#include<limits.h>
#ifdef FT_PRINTF
#define F(...) \
res = ft_printf(__VA_ARGS__)
#else
#define F(...) \
res = printf(__VA_ARGS__)
#endif

int	main(void)
{
	int res;
	F("test c   : %c|<- end\n", 'c');
	F("test s   : %s|<- end\n", "hello");
	F("test p 1 : %p|<- end\n", "hello");
	F("test p 2 : %p|<- end\n", "world");
	F("test p 3 : %p|<- end\n", NULL);
	void *p = malloc(1);
	F("test p 4 : %p|<- end\n", p);
	free(p);
	F("test d 1 : %d|<- end\n", 42);
	F("test d 2 : %d|<- end\n", -42);
	F("test d 3 : %d|<- end\n", 0);
	F("test d 4 : %d|<- end\n", INT_MIN);
	F("test d 5 : %d|<- end\n", INT_MAX);
	F("test i 1 : %i|<- end\n", 42);
	F("test i 2 : %i|<- end\n", -42);
	F("test i 3 : %i|<- end\n", 0);
	F("test i 4 : %i|<- end\n", INT_MIN);
	F("test i 5 : %i|<- end\n", INT_MAX);
	F("test u 1 : %u|<- end\n", 42);
	F("test u 2 : %u|<- end\n", -42);
	F("test u 3 : %u|<- end\n", 0);
	F("test u 4 : %u|<- end\n", INT_MIN);
	F("test u 5 : %u|<- end\n", INT_MAX);
	F("test o 1 : %o|<- end\n", 42);
	F("test o 2 : %o|<- end\n", -42);
	F("test o 3 : %o|<- end\n", 0);
	F("test o 4 : %o|<- end\n", INT_MIN);
	F("test o 5 : %o|<- end\n", INT_MAX);
	F("test x 1 : %x|<- end\n", 42);
	F("test x 2 : %x|<- end\n", -42);
	F("test x 3 : %x|<- end\n", 0);
	F("test x 4 : %x|<- end\n", INT_MIN);
	F("test x 5 : %x|<- end\n", INT_MAX);
	F("test X 1 : %X|<- end\n", 42);
	F("test X 2 : %X|<- end\n", -42);
	F("test X 3 : %X|<- end\n", 0);
	F("test X 4 : %X|<- end\n", INT_MIN);
	F("test X 5 : %X|<- end\n", INT_MAX);
}
#endif
