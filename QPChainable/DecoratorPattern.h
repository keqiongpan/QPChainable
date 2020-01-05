//
//  DecoratorPattern.h
//  QPChainable
//
//  Created by keqiongpan@163.com on 2019/8/5.
//  Copyright © 2019 Qiongpan Ke. All rights reserved.
//

#import <Foundation/Foundation.h>


/**
 *  对于支持链式表达式的对象，一般需要关注下面三个方面的点：
 *
 *      1、数据，即链式表达式函数所操作的数据，或与之相关的数据；
 *      2、函数，即链式表达式函数，也是类的成员方法；
 *      3、继承，即链式表达式函数是否能被继承，且支持与子类链式表达式函数混用？
 *
 *  对于上述三个方面，除了链式表达式函数是必须需要具备的点之外，数据则是可选的，
 *  而链式表式函数的继承也不是必须需要支持的。对于数据和函数比较容易理解，但为什
 *  么继承也是需要关注的点呢？这是因为，如果要想使父类的链式表达式函数与子类的链
 *  式表达式函数连起来进行混用，就要求这些函数返回的对象都必须是最终的实例类型，
 *  否则在调用完其中一个链式表达式函数后，就只能根据该链式表达式函数所声明的返回
 *  值对象类型来调用其成员函数。所以父类的链式表达式函数在声明的时候就要指定返回
 *  最终的实例类型，这就要求最终的实例类型必须是通过泛型参数传递给父类的。
 *
 *  为方便用户搭建支持链式表达式函数的类或类体系，我们基于装饰者模式（Decorator
 *  Pattern）设计了一组抽象基类，用以方便用户继承来支持链式表达式函数。在这组装
 *  饰者模式基类下，封装被操作数据的类被称为具象模型类（Concrete Model Class），
 *  而封装链式表达式函数的类被称为具象构造类（Concrete Maker Class），而包裹这两
 *  个相互对应类的、被用户直接持有和调用的是装饰类（Decorator Class）。
 *
 *  这组基于装饰者模式的抽象基类，不是每个都是必须的，特别是具象模型类和装饰类，
 *  在不同场景下，可能是体系外的类，或与具象构造类为同一个类。
 *
 *  下面是一些常见的场景下，对这组基于装饰者模式的抽象基类的使用方法：
 *
 *      1、扩展现有类，为其添加若干链式表达式函数，用于配置自身属性。这种情况下
 *      直接使用<QPChainable/Core.h>的内容进行支持即可，不需要用到这套抽象基类：
 *
 *      例如，扩展UIView类，添加若干链式表达式函数配置视图样式：
 *
 *      \@interface UIView (ForChainedFunctions)
 *      - QP_CHAINABLE_DECLARE(setCenterTo, (CGPoint center), UIView *);
 *      - QP_CHAINABLE_DECLARE(setBackgroundColorTo, (UIColor *backgroundColor), UIView *);
 *      ...
 *      \@end
 *
 *      2、数据为现有类，新增构造类提供链式表达式函数以对该数据类进行配置。这种
 *      况下，由于数据类型确定，对应的链式表达式函数一般不要求支持继承混用；
 *
 *      例如，创建QPViewMaker类来配置UIView对象，且QPViewMaker中的链式表达式函数
 *      不需要支持继承混用：
 *
 *      \@class QPViewMaker;
 *      \@interface QPViewMaker : QPConcreteChainableMaker<QPViewMaker *, UIView *>
 *      - QP_CHAINABLE_DECLARE(center, (CGPoint center), QPViewMaker *);
 *      - QP_CHAINABLE_DECLARE(backgroundColor, (UIColor *backgroundColor), QPViewMaker *);
 *      ...
 *      \@end
 *
 *      UIView *view = ...;
 *      QPViewMaker *viewMaker = [[QPViewMaker alloc] initWithData:view];
 *
 *      // 相当于viewMaker.backgroundColor([UIColor redColor]);
 *      viewMaker.make.backgroundColor([UIColor redColor]);
 *
 *      // 相当于UIColor *viewBackgroundColor = view.backgroundColor;
 *      UIColor *viewBackgroundColor = viewMaker.data.backgroundColor;
 *
 *      3、全新设计的有数据属性及链式表达式函数的类或类体系，这种情况下情况下，
 *      数据属性放在具象模型类（Concrete Model Class），而链式表达式函数放在具象
 *      构造类（Concrete Maker Class），两者分别有各自的继承体系，仅在同一继承层
 *      级上有对应关系，且在该层级上使用的装饰类（Decorator Class）进行关联，装
 *      饰类的data属性指向具象模型类，make属性指向具象构造类（即self，因为装饰类
 *      就是具象模型类的子类）。
 *
 *      例如，构建一套描述表单项内容的模型类，则抽象表单项（QPFieldModel）与文本
 *      表单项（QPTextFieldModel）的关系可以被设计为如下方式：
 *
 *      //
 *      // QPFieldModel.h
 *      //
 *
 *      \@interface QPConcreteFieldModel : QPConcreteChainableModel
 *      \@property (nonatomic, copy) NSString *title;
 *      \@property (nonatomic, copy) NSString *value;
 *      \@end
 *
 *      \@interface QPConcreteFieldMaker<instancetype, datatype> : QPConcreteChainableMaker<instancetype, datatype>
 *      - QP_CHAINABLE_DECLARE(title, (NSString *title));
 *      - QP_CHAINABLE_DECLARE(value, (NSString *value));
 *      \@end
 *
 *      \@class QPFieldModel;
 *      \@interface QPFieldModel : QPConcreteFieldMaker<QPFieldModel *, QPConcreteFieldModel *>
 *      \@end
 *
 *      //
 *      // QPTextFieldModel.h
 *      //
 *
 *      \@interface QPConcreteTextFieldModel : QPConcreteFieldModel
 *      \@property (nonatomic, assign) NSUInteger maximumLength;
 *      \@end
 *
 *      \@interface QPConcreteTextFieldMaker<instancetype, datatype> : QPConcreteFieldMaker<instancetype, datatype>
 *      - QP_CHAINABLE_DECLARE(limitBy, (NSUInteger maximumLength));
 *      \@end
 *
 *      \@class QPTextFieldModel;
 *      \@interface QPTextFieldModel : QPConcreteTextFieldMaker<QPTextFieldModel *, QPConcreteTextFieldModel *>
 *      \@end
 *
 *      //
 *      // main.m
 *      //
 *
 *      QPTextFieldModel *nameField = [[QPTextFieldModel alloc] init];
 *      nameField.make.title(@"姓名").limitBy(4);
 */


NS_ASSUME_NONNULL_BEGIN


@class QPChainableModel;
@class QPConcreteChainableModel;
@class QPConcreteChainableMaker<instancetype, datatype>;


/**
 *  具象模型类（Concrete Model Class）。
 */
@interface QPConcreteChainableModel : NSObject

@end


/**
 *  具象构造类（Concrete Maker Class）。
 */
@interface QPConcreteChainableMaker<instancetype, datatype> : NSObject

- (instancetype)initWithData:(datatype)data;
- (datatype)data;
- (instancetype)make;

@end


/**
 *  装饰类（Decorator Class）。
 */
@interface QPChainableModel : QPConcreteChainableMaker<QPChainableModel *, QPConcreteChainableModel *>

@end


NS_ASSUME_NONNULL_END
