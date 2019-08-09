//
//  Core.h
//  QPChainable
//
//  Created by keqiongpan@163.com on 2019/8/1.
//  Copyright © 2019 Qiongpan Ke. All rights reserved.
//


#pragma mark - QP_CHAINABLE_DECLARE(name, arguments[, returntype])

/**
 *  在类声明体中添加支持链式表达式的成员函数。
 *
 *  @param name         函数名，表示链式表达式中的一个操作。
 *  @param arguments    函数的形参列表，需要使用`()'括起来。
 *                      如果没有参数，可使用`(void)'来表示。
 *  @param returntype   函数的返回值类型，可选参数，默认为instancetype。
 *
 *  @example 使用示例，如下：
 *      \@interface QPPacketMaker : NSObject
 *      - QP_CHAINABLE_DECLARE(length, (void), NSUInteger);
 *      - QP_CHAINABLE_DECLARE(append, (QPMultipartData *data), QPPacketMaker *);
 *      - QP_CHAINABLE_DECLARE(send, (void), QPPacketMaker *);
 *      \@end
 *
 *  @note 注意， returntype 一般为当前链式表达式函数声明所在类的实例类型。如果要
 *      想使子类实现的链式表达式函数与父类实现的链式表达式函数混用，可以将子类与
 *      父类均改造为泛型类，并将面向最终使用者的实例类型通过泛型参数传递给子类和
 *      父类，子类和父类均使用该实例类型作为链式表达式函数的返回值类型，在函数实
 *      现时将self返回即可。然后为子类或父类实现面向最终使用者的、非泛型的、无成
 *      员（方法、属性、实例变量等）的装饰子类（Decorator Subclass），并将其基类
 *      的泛型形参指定为该装饰子类的实例类型。这样，原来子类与父类实现的链式表达
 *      式函数就可以进行混用了，并且这些链式表达式函数也能被后代子类继续继承。但
 *      需要注意到，这样处理之后，在类的继承体系上会多出一些无成员的装饰子类，但
 *      由于没有额外的成员（方法、属性、实例变量等），在理论上是可以将父类实例指
 *      针强制转换为装饰子类实例指针使用的。
 *
 *      详细示例如下：
 *
 *      \@interface QPConcreteBase<instancetype> : NSObject
 *      - QP_CHAINABLE_DECLARE(setValue, (id value));
 *      \@end
 *
 *      \@interface QPConcreteChild<instancetype> : QPConcreteBase<instancetype>
 *      - QP_CHAINABLE_DECLARE(dump, (void));
 *      \@end
 *
 *      \@class QPChild;
 *      \@interface QPChild : QPConcreteChild<QPChild *>
 *      \@end
 *
 *      QPChild.new.setValue(...).dump();
 */
#ifndef QP_CHAINABLE_DECLARE

#define QP_CHAINABLE_DECLARE(... /* name, arguments[, returntype] */) \
/**/    _QP_CHAINABLE_DECLARE_internal(__VA_ARGS__, N3, N2, N1)

#define _QP_CHAINABLE_DECLARE_internal(v1, v2, v3, suffix, ...) \
/**/    _QP_CHAINABLE_DECLARE_supports_only_2_or_3_arguments_##suffix(v1, v2, v3)

#define _QP_CHAINABLE_DECLARE_supports_only_2_or_3_arguments_N2(name, arguments, ...) \
/**/    (instancetype (^) arguments)name

#define _QP_CHAINABLE_DECLARE_supports_only_2_or_3_arguments_N3(name, arguments, returntype, ...) \
/**/    (returntype (^) arguments)name

#endif


#pragma mark - QP_CHAINABLE_IMPLEMENTATION(name, arguments[, returntype], body)

/**
 *  在类实现体中添加支持链式表达式的成员函数。
 *
 *  @param name         函数名，表示链式表达式中的一个操作。
 *  @param arguments    函数的形参列表，需要使用`()'括起来。
 *                      如果没有参数，可使用`(void)'来表示。
 *  @param returntype   函数的返回值类型，可选参数，默认为instancetype。
 *  @param body         函数的实现体，使用`({'和`})'括起来。
 *
 *  @example 使用示例，如下：
 *      \@implementation QPPacketMaker
 *      - QP_CHAINABLE_IMPLEMENTATION(length, (void), NSUInteger, ({
 *          return self.data.length;
 *      }));
 *      \@end
 */
#ifndef QP_CHAINABLE_IMPLEMENTATION

#define QP_CHAINABLE_IMPLEMENTATION(... /* name, arguments[, returntype], body */) \
/**/    _QP_CHAINABLE_IMPLEMENTATION_internal(__VA_ARGS__, N4, N3, N2, N1)

#define _QP_CHAINABLE_IMPLEMENTATION_internal(v1, v2, v3, v4, suffix, ...) \
/**/    _QP_CHAINABLE_IMPLEMENTATION_supports_only_3_or_4_arguments_##suffix(v1, v2, v3, v4)

#define _QP_CHAINABLE_IMPLEMENTATION_supports_only_3_or_4_arguments_N3(name, arguments, body, ...) \
/**/    (id (^) arguments)name { \
/**/        return ^id arguments { \
/**/            do { \
/**/                _You_need_wrap_the_implementation_body_with_parentheses_ body; \
/**/            } while (0); \
/**/            return self; \
/**/        }; \
/**/    }

#define _QP_CHAINABLE_IMPLEMENTATION_supports_only_3_or_4_arguments_N4(name, arguments, returntype, body, ...) \
/**/    (returntype (^) arguments)name { \
/**/        return ^returntype arguments { \
/**/            _You_need_wrap_the_implementation_body_with_parentheses_ body; \
/**/        }; \
/**/    }

#endif


#pragma mark - ({ ... do something here ... })

/**
 *  与QP_CHAINABLE_IMPLEMENTATION搭配使用，将函数体括起来。主要是方便预编译程序
 *  将其识别为一个整体的参数值传递给宏函数内部，避免因代码中的英文逗号“,”被识
 *  别为宏函数的多个参数值。
 *
 *  @param ...  可以是单条语句，或用花括号`{}'括起来的语句块。
 *
 *  @see QP_CHAINABLE_IMPLEMENTATION
 */
#ifndef _You_need_wrap_the_implementation_body_with_parentheses_
#define _You_need_wrap_the_implementation_body_with_parentheses_(...) \
/**/    __VA_ARGS__
#endif
