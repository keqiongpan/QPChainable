//
//  DecoratorPattern.h
//  QPChainable
//
//  Created by keqiongpan@163.com on 2019/8/5.
//  Copyright © 2019 Qiongpan Ke. All rights reserved.
//

#import <Foundation/Foundation.h>


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
@interface QPChainableModel : QPConcreteChainableMaker<QPConcreteChainableMaker *, QPConcreteChainableModel *>

@end


NS_ASSUME_NONNULL_END
