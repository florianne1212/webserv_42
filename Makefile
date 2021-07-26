# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: fcoudert <fcoudert@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/12/16 08:17:15 by ldutriez          #+#    #+#              #
#    Updated: 2021/07/26 10:27:51 by fcoudert         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= webserv

CC =		clang++

SRC_DIR = 	$(shell find srcs -type d)
INC_DIR = 	$(shell find srcs -type d)
OBJ_DIR = obj

vpath %.cpp $(foreach dir, $(SRC_DIR), $(dir):)

#SRC = $(foreach dir, $(SRC_DIR), $(foreach file, $(wildcard $(dir)/*.cpp), $(notdir $(file))))
SRC = main.cpp Config.cpp Server.cpp Routes.cpp ASocket.cpp ClientSocket.cpp FDList.cpp ListeningSocket.cpp parseHeaderFields.cpp \
parseRequest.cpp parseBody.cpp Message.cpp parseChunkedBody.cpp request.cpp response.cpp deleteMethod.cpp mime.cpp listmime.cpp \
httpStatus.cpp getMethod.cpp File.cpp IMiddleware.cpp MiddlewareChain.cpp MethodMiddleware.cpp  checkRedir.cpp \
isConnected.cpp checkRequest.cpp ManageMiddleware.cpp postMethod.cpp cgiHandler.cpp buffer.cpp checkRoot.cpp

OBJ = $(addprefix $(OBJ_DIR)/, $(SRC:%.cpp=%.o))

CFLAGS = -Wall -Wextra -Werror -std=c++98 -g3 #-fsanitize=address

IFLAGS = $(foreach dir, $(INC_DIR), -I$(dir))
# Colors

_GREY=	$'\033[30m
_RED=	$'\033[31m
_GREEN=	$'\033[32m
_YELLOW=$'\033[33m
_BLUE=	$'\033[34m
_PURPLE=$'\033[35m
_CYAN=	$'\033[36m
_WHITE=	$'\033[37m

all:			$(NAME)

show:
	@echo "$(_BLUE)SRC :\n$(_YELLOW)$(SRC)$(_WHITE)"
	@echo "$(_BLUE)OBJ :\n$(_YELLOW)$(OBJ)$(_WHITE)"
	@echo "$(_BLUE)CFLAGS :\n$(_YELLOW)$(CFLAGS)$(_WHITE)"
	@echo "$(_BLUE)IFLAGS :\n$(_YELLOW)$(IFLAGS)$(_WHITE)"
	@echo "$(_BLUE)INC_DIR :\n$(_YELLOW)$(INC_DIR)$(_WHITE)"

$(OBJ_DIR)/%.o : %.cpp
				@echo "Compiling $(_YELLOW)$@$(_WHITE) ... \c"
				@mkdir -p $(OBJ_DIR)
				@$(CC) $(CFLAGS) $(IFLAGS) -o $@ -c $<
				@echo "$(_GREEN)DONE$(_WHITE)"

$(NAME): 		$(INC_DIR) $(OBJ) Makefile
				@echo "-----\nCreating Executable $(_YELLOW)$@$(_WHITE) ... \c"
				@$(CC) $(CFLAGS) $(OBJ) -o $(NAME)
				@echo "$(_GREEN)DONE$(_WHITE)\n-----"

re:				fclean all

clean:
				@echo "$(_WHITE)Deleting Object $(_YELLOW)$(OBJ_DIR)$(_WHITE) ... \c"
				@rm -rf $(OBJ_DIR)
				@echo "$(_GREEN)DONE$(_WHITE)\n-----"

fclean:			clean
				@echo "Deleting Binary File $(_YELLOW)$(NAME)$(_WHITE) ... \c"
				@rm -f $(NAME)
				@echo "$(_GREEN)DONE$(_WHITE)\n-----"

donuts :
				@gcc donuts.c -o donuts
				@./donuts

.PHONY: all clean flcean re show donuts
